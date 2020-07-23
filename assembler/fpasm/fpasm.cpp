#include "fpasm.h"

void FirstPass::removeComments(QStringList &param) {
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
        QStringList splittedLine = p.split(QRegExp(" |\\,"), QString::SkipEmptyParts);

        //Handling the negative sign by taking the tow's complement
        //to be moved to a function.
        p = "";
        for(auto &x : splittedLine) {
            if(getOperandType(x) == OperandType::NegImmed8 ||
                    getOperandType(x) == OperandType::NegImmed16)
            {
                x = signHandler(static_cast<QString>(x), getOperandType(x));
            }
            else if(getOperandType(x) == OperandType::Char) {
                x.remove("'");
                QChar buf = x.at(0);
                x = QString::number(buf.unicode(), 16);
            }
            p+=x+' ';
        }

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
