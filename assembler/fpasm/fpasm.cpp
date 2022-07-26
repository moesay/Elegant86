#include "fpasm.h"
#include <QScriptEngine>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <memory>
#include <string>

InstRet_T FirstPhase::assemble(const QString& line) {
    return Get().Iassemble(line);
}

std::tuple<QStringList, QList<Error_T>> FirstPhase::validate(const QStringList& param) {
    return Get().Ivalidate(param);
}

void FirstPhase::removeComments(QStringList &param) noexcept {
    param.append(";"); //segfault
    param.erase(
            std::remove_if(std::begin(param), std::end(param), [](QString &p) {
                return p.startsWith(';');}
                ));

    std::transform(std::begin(param), std::end(param), std::begin(param), [](QString &x){
            int mark = x.indexOf(";");
            int mark2 = x.indexOf('"');
            if(mark < mark2) return x.left(mark);
            else if(mark2 == -1 && mark != -1) return x.left(mark);
            return x;
            });
}

/*
 * Labels definition : .LBL:
 * Labels calling : jmp .LBL
 */
QList<Error_T> FirstPhase::getLabels(QStringList &param) {
    /*
     * @INLINE LABELS ARE NOT SUPPORTED, YET...OR NEVER!
     * should be replaced by cs:[addr] (!!) : maybe not! this should be resolved when implemeting the cpu
     */
    QList<Error_T> ret;
    int bytesCount = 0;
    for (auto p : param) {
        auto [machCode, success, ident] = assemble(p);
        bytesCount += machCode.length() / 2;
        if(ident == "LBL") {
            //If this label already in the labels list, this is a double definition
            if(Labels::labelExists(p)) {
                ret.append(std::make_tuple("Redefinition of a Label", p, 0));
                continue;
            }
            Labels::addLabel(bytesCount, p);
        }
    }
    return ret;
}

std::tuple<QStringList, QList<Error_T>> FirstPhase::Ivalidate(const QStringList& code) {

    QRegExp negRegx("-[0-9a-fA-F]+");
    QRegExp charRegx("'.'|\".\"");
    QRegExp evalRegx("(\\w*[+-]\\w*)+");
    QList<Error_T> ret;
    int lineNumber=0;
    readyCode = code;
    removeComments(readyCode);

    ret = getLabels(readyCode);

    /*
     * @Rules :
     *  1- Check that each line begins with an instruction
     *  2- If there is a negative number, process it
     *  3- Evaluate the addresses mathematical operations
     */
    for (auto &p : readyCode) {
        ++lineNumber;
        if(p.isEmpty()) continue;
        bool isLabel = Labels::isLableDef(p);

        int pos = 0;
        while((pos = evalRegx.indexIn(p, pos)) != -1) {
            QString temp = evalRegx.cap(0);
            std::optional<QString> evaluatedExp = eval(temp);
            if(evaluatedExp != std::nullopt)
                p.replace(pos, evalRegx.matchedLength(), *evaluatedExp);
            pos+=evalRegx.matchedLength();
        }
        pos ^= pos;
        //Handling the negative sign by taking the tow's complement
        //to be moved to a function.
        while((pos = negRegx.indexIn(p, pos)) != -1) {
            if(p.at(pos-1) != ' ' && p.at(pos-negRegx.matchedLength()-1) != ',')
                p.replace(pos, negRegx.matchedLength(), "+" + signHandler(negRegx.cap(0), getOperandType(negRegx.cap(0))).value_or(""));
            else
                p.replace(pos, negRegx.matchedLength(), signHandler(negRegx.cap(0), getOperandType(negRegx.cap(0))).value_or(""));
            pos+=negRegx.matchedLength();
        }
        pos ^= pos;
        while((pos = charRegx.indexIn(p, pos)) != -1) {
            QChar ch = charRegx.cap(0).at(1);
            p.replace(pos, charRegx.matchedLength(), QString::number(ch.unicode(), 16));
            pos+=charRegx.matchedLength();
        }

        QStringList splittedLine = p.split(QRegExp(" |\\,"), Qt::SplitBehaviorFlags::SkipEmptyParts);

        auto instResult = std::find(std::begin(instructionsLUT),
                std::end(instructionsLUT),splittedLine.first().toUpper().toStdString());

        if((splittedLine.count() >= 2) && (getOperandType(splittedLine.at(1)) == Label)) {
            if(Labels::labelExists(splittedLine.at(1)) == false) {ret.append(std::make_tuple(QString("Undefined Label"), p, lineNumber)); continue;}
        }

        if(instResult == std::end(instructionsLUT) && !isLabel) {ret.append(std::make_tuple(QString("Unknown Instruction"), p, lineNumber)); continue;}
    }

    return {readyCode, ret};
}

std::optional<QString> FirstPhase::eval(QString param) {

    param = "0x0+"+param;

    QStringList buf;
    QScriptEngine engine;
    QString ret;

    for(auto reg : SegRegs) {
        engine.globalObject().setProperty(reg, "0");
        if(param.toUpper().contains(reg)) {buf.append(reg); param.remove(reg, Qt::CaseInsensitive);}
    }
    for(auto reg : Regs8) {
        engine.globalObject().setProperty(reg, "0");
        if(param.toUpper().contains(reg)) {buf.append(reg); param.remove(reg, Qt::CaseInsensitive);}
    }
    for(auto reg : Regs16) {
        engine.globalObject().setProperty(reg, "0");
        if(param.toUpper().contains(reg)) {buf.append(reg); param.remove(reg, Qt::CaseInsensitive);}
    }

    //Adding the hex prefix, easy but not safe. TBE.
    param.replace('+', "+0x0");
    param.replace('-', "-0x0");

    if(param.contains("++")) param.remove("++");
    if(param.contains("--")) param.remove("--");

    param = engine.evaluate(param).toString();

    if(engine.hasUncaughtException())
        return std::nullopt;

    ret = numToHexStr(param.toInt(), OutputSize::Dynamic, Sign::Neg);
    if(ret == 0) return "";

    param="";

    for(const auto &reg : buf)
        param+=reg+"+";

    param.append(ret);
    return param;
}

InstRet_T FirstPhase::Iassemble(const QString& param) {
    std::unique_ptr<Base> b;
    QString inst = param.trimmed().split(" ").at(0).toUpper();
    //Because we need to assemble to code to calculate the offset of the label.
    //we should be ok with lables defs
    if(Labels::isLableDef(inst))
        return {"", true, "LBL"};

    if(inst == "MOV") {
        b = std::make_unique<Mov>(param);
        return b->process();
    }
    else if(inst == "ADD") {
        b = std::make_unique<Add>(param);
        return b->process();
    }
    else if(inst == "PUSH") {
        b = std::make_unique<Push>(param);
        return b->process();
    }
    else if(inst == "POP") {
        b = std::make_unique<Pop>(param);
        return b->process();
    }
    else if(inst == "AND") {
        b = std::make_unique<And>(param);
        return b->process();
    }
    else if(std::any_of(std::begin(NoOpInsts), std::end(NoOpInsts), [=](const std::string& x) {return x == inst.toStdString();})) {
        b = std::make_unique<No_OP_Inst>(param);
        return b->process();
    }
    else if(std::any_of(std::begin(instructionsLUT), std::end(instructionsLUT), [=](const std::string& x) {return x == inst.toStdString();})) {
        return {"", false, inst+ " Instruction hasn't been implemented yet."};
    }
    return {"", false, "Unknown Instruction"};
}
