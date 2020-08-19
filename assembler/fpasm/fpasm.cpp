#include "fpasm.h"
#include <QScriptEngine>

void FirstPass::removeComments(QStringList &param) noexcept {
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

QList<label> FirstPass::getLabels(const QStringList &param) {
    //INLINE LABELS ARE NOT SUPPORTED
    QList<label> ret;

    for (auto p : param) {
        if(p.contains(":")) {
            QString buf = p;
            buf.remove(':');
            if(p.endsWith(":")) ret.append(std::make_tuple(0, p, buf));
            else {
                //to distinguish between the : in strings
                int mark = p.indexOf('"');
                int mark2 = p.indexOf(":");
                if(mark < mark2) continue;
                else ret.append(std::make_tuple(0, p, buf));
            }
        }
    }
    return ret;
}

QList<error> FirstPass::validate(const QStringList& code) {

    QRegExp negRegx("-[0-9a-fA-F]+");
    QRegExp charRegx("'.'|\".\"");
    QRegExp evalRegx("(\\w*[+-]\\w*)+");
    QList<error> ret;
    int lineNumber=0;
    readyCode = code;
    removeComments(readyCode);
    lbls = getLabels(readyCode);

    //Rule :
    //  1- Check that each line starts with an instruction
    //  2- Validates the operands of each instruction
    //  3- If there is a negative number, process it
    for (auto &p : readyCode) {
        ++lineNumber;
        if(p.isEmpty()) continue;
        bool isLabel = p.endsWith(':');

        //Handling the negative sign by taking the tow's complement
        //to be moved to a function.
        int pos = 0;
        while((pos = negRegx.indexIn(p, pos)) != -1) {
            if(p.at(pos-1) != ' ' && p.at(pos-negRegx.matchedLength()-1) != ',')
                p.replace(pos, negRegx.matchedLength(), "+"+signHandler(negRegx.cap(0), getOperandType(negRegx.cap(0))));
            else
                p.replace(pos, negRegx.matchedLength(), signHandler(negRegx.cap(0), getOperandType(negRegx.cap(0))));
            pos+=negRegx.matchedLength();
        }
        pos ^= pos;
        while((pos = evalRegx.indexIn(p, pos)) != -1) {
            QString temp = evalRegx.cap(0);
            if(eval(temp))
                p.replace(pos, evalRegx.matchedLength(), temp);
            pos+=evalRegx.matchedLength();
        }
        pos ^= pos;
        while((pos = charRegx.indexIn(p, pos)) != -1) {
            QChar ch = charRegx.cap(0).at(1);
            p.replace(pos, charRegx.matchedLength(), QString::number(ch.unicode(), 16));
            pos+=charRegx.matchedLength();
        }

        QStringList splittedLine = p.split(QRegExp(" |\\,"), QString::SkipEmptyParts);
        auto cmpLine = std::make_tuple(splittedLine.at(0).toUpper(),
                (splittedLine.count() >= 2 ? getOperandType(splittedLine.at(1)) : NOP),
                (splittedLine.count() >= 3 ? getOperandType(splittedLine.at(2)) : NOP));

        auto operandsResult = std::find(std::begin(operandsLUT), std::end(operandsLUT), cmpLine);

        auto instResult = std::find(std::begin(instructionsLUT),
                std::end(instructionsLUT),splittedLine.first().toUpper());

        QList<label>::iterator lblFindResult;

        if((splittedLine.count() >= 2) && (getOperandType(splittedLine.at(1)) == Label)) {
            lblFindResult = std::find(std::begin(lbls), std::end(lbls), std::make_tuple(0, splittedLine.at(1)+':', splittedLine.at(1)));
        }

        if(instResult == std::end(instructionsLUT) && !isLabel) {ret.append(std::make_tuple(QString("Unknown Instruction"), p, lineNumber)); continue;}
        if(operandsResult == std::end(operandsLUT) && !isLabel) {ret.append(std::make_tuple(QString("Invalid Operands"), p, lineNumber)); continue;}
        if(lblFindResult == std::end(lbls)) {ret.append(std::make_tuple(QString("Undefined Label"), p, lineNumber)); continue;}
    }

    return ret;
}

bool FirstPass::eval(QString& param) {
    QString bufS = param;

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
    if(engine.hasUncaughtException()) {
        param = bufS;
        return false;
    }
    ret = hexToStr(param.toInt(), HexType::OpCode, Sign::Neg);
    param="";
    for(const auto &reg : buf) {
        param+=reg+"+";
    }
    param.append(ret);
    return true;
}
