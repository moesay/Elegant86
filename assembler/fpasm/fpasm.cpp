#include "fpasm.h"
#include <QScriptEngine>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

std::vector<Error_T> FirstPhase::errors;
std::vector<output_t> FirstPhase::assemble() {
    return Get().Iassemble();
}

void FirstPhase::validate(const QStringList& param) {
    return Get().Ivalidate(param);
}

void FirstPhase::removeComments() noexcept {
    codeInfo.push_back({0, 0, "", "", 0});
    codeInfo.erase(
            std::remove_if(std::begin(codeInfo), std::end(codeInfo), [](info_t &p) {
                return p.code.starts_with(';') == true;
                }));

    std::transform(std::begin(codeInfo), std::end(codeInfo), std::begin(codeInfo), [](info_t &x) -> info_t {
            auto [size, relAddr, code, machCode, ln] = x;
            int mark = code.find_first_of(";");
            int mark2 = code.find_first_of('"');
            if(mark < mark2) return {size, relAddr, code.substr(0, mark2), machCode, ln};
            else if(mark2 == -1 && mark != -1) return {size, relAddr, code.substr(0, mark), machCode, ln};
            return x;
            });
}

/*
 * Labels definition : .LBL:
 * Labels calling : jmp .LBL
 */
void FirstPhase::getLabels() {
    /*
     * @INLINE LABELS ARE NOT SUPPORTED, YET...OR NEVER!
     */
    int offset = 0;
    for (auto &p : codeInfo) {
        auto [machCode, success, ident] = preAsm(QString::fromStdString(p.code), true);
        offset += machCode.length() / 2;
        if(ident == "LBL") {
            //If this label already in the labels list, this is a double definition
            if(Labels::labelExists(QString::fromStdString(p.code))) {
                errors.push_back(std::make_tuple("Redefinition of a Label", QString::fromStdString(p.code), 0));
                continue;
            }
            Labels::addLabel(offset, QString::fromStdString(p.code));
        }
        else if(ident == "JMP") {
            /* p.append(" " + QString::number(offset)); */
        }
        p.size = machCode.length();
        p.relAddr = offset;
        p.machCode = machCode.toStdString();
    }
}

void FirstPhase::Ivalidate(const QStringList& code) {

    errors.clear();
    codeInfo.clear();

    //load the code into codeInfo
    for(int i = 0; i < code.size(); i++) {
        codeInfo.push_back({0, 0, code.at(i).toStdString(), "", i+1});
    }
    removeComments();
    QRegExp negRegx("-[0-9a-fA-F]+");
    QRegExp charRegx("'.'|\".\"");

    getLabels();

    /*
     * @Rules :
     *  1- Check that each line begins with an instruction
     *  2- If there is a negative number, process it
     *  3- Evaluate the addresses mathematical operations
     */
    for (auto &entery : codeInfo) {
        QString p = QString::fromStdString(entery.code);
        if(p.isEmpty()) continue;
        bool isLabel = isLableDef(p);

        int pos = 0;
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

        entery.code = p.toStdString();

        auto instResult = std::find(std::begin(instructionsLUT),
                std::end(instructionsLUT),splittedLine.first().toUpper().toStdString());

        if((splittedLine.count() >= 2) && (getOperandType(splittedLine.at(1)) == Label)) {
            if(Labels::labelExists(splittedLine.at(1)) == false) {errors.push_back(std::make_tuple(QString("Undefined Label"), p, entery.ln)); continue;}
        }

        if(instResult == std::end(instructionsLUT) && !isLabel) {errors.push_back(std::make_tuple(QString("Unknown Instruction"), p, entery.ln)); continue;}
    }
}

std::vector<output_t> FirstPhase::Iassemble() {

    std::vector<output_t> output;
    std::unique_ptr<Base> b;

    for(const auto& entery : codeInfo) {
        QString inst = QString::fromStdString(entery.code).trimmed().split(" ").at(0).toUpper();
        QString param = QString::fromStdString(entery.code);

        if(inst.isEmpty()) continue;
        if(isLableDef(inst)) continue;

        if(inst == "MOV") {
            b = std::make_unique<Mov>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(inst == "ADD") {
            b = std::make_unique<Add>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(inst == "PUSH") {
            b = std::make_unique<Push>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(inst == "POP") {
            b = std::make_unique<Pop>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(inst == "AND") {
            b = std::make_unique<And>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(std::any_of(std::begin(NoOpInsts), std::end(NoOpInsts), [=](const std::string& x) {return x == inst.toStdString();})) {
            b = std::make_unique<No_OP_Inst>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(std::any_of(std::begin(JMPS), std::end(JMPS), [=](const std::string& x) {return x == inst.toStdString();})) {
            b = std::make_unique<Jmps>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(inst == "JMP") {
            b = std::make_unique<Jmp>(param);
            b->setInfo(entery);
            auto[mach, state, err] = b->process();
            output.push_back({entery.code, mach.toStdString(), err.toStdString(), entery.ln, state});
        }
        else if(std::any_of(std::begin(instructionsLUT), std::end(instructionsLUT), [=](const std::string& x) {return x == inst.toStdString();})) {
            output.push_back({entery.code, "", inst.toStdString() + " Instruction hasn't been implemented yet.", entery.ln, false});
        }
        else
            output.push_back({entery.code, "", "Unknown Instruction.", entery.ln, false});
    }
    return output;
}

InstRet_T FirstPhase::preAsm(const QString& param, bool fetch) {
    std::unique_ptr<Base> b;
    QString inst = param.trimmed().split(" ").at(0).toUpper();
    //Because we need to assemble to code to calculate the offset of the label.
    //we should be ok with lables defs
    if(!fetch) goto start;

    if(std::any_of(std::begin(JMPS), std::end(JMPS), [=](const std::string& x) {return x == inst.toStdString();}))
        return {"0000", true, "JMP"};
    if(inst == "JMP") {
        QString type = param.trimmed().split(" ").at(1).toUpper();
        if(type == "FAR")
            return {"0000000000", true, "JMP"};
        else if(type == "LONG")
            return {"000000", true, "JMP"};
        else
            return {"0000", true, "JMP"};
    }
start:
    if(isLableDef(inst))
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
    else if(!fetch && std::any_of(std::begin(JMPS), std::end(JMPS), [=](const std::string& x) {return x == inst.toStdString();})) {
        b = std::make_unique<Jmps>(param);
        return b->process();
    }
    else if(!fetch && inst == "JMP") {
        b = std::make_unique<Jmp>(param);
        return b->process();
    }
    else if(std::any_of(std::begin(instructionsLUT), std::end(instructionsLUT), [=](const std::string& x) {return x == inst.toStdString();})) {
        return {"", false, inst+ " Instruction hasn't been implemented yet."};
    }
    return {"", false, "Unknown Instruction"};
}
