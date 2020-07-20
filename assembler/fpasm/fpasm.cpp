#include "fpasm.h"

FirstPass& FirstPass::Get() {
    static FirstPass instance;
    return instance;
}

void FirstPass::removeComments(QStringList &param) {
    /* param.append(";Added to prevent an ambigous sigfaut!"); */
    /* param.erase( */
    /*         std::remove_if(std::begin(param), std::end(param), */
    /*             [](QString const &p) { */
    /*             return p.startsWith(';'); */
    /*             }) */
    /*         ); */
    std::transform(std::begin(param), std::end(param), std::begin(param), [](QString &x){
            if(x.startsWith(';')) return QString("");

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
            if(p.endsWith(":")) ret.append(std::make_tuple(0, p, p.remove(':')));
            else {
                int mark = p.indexOf('"');
                int mark2 = p.indexOf(":");
                if(mark < mark2) continue;
                else ret.append(std::make_tuple(0, p, p.remove(':')));
            }
        }
    }
    return ret;
}

QList<error> FirstPass::validate(const QStringList& code) {
    QList<error> ret;
    QStringList pureCode = code;
    int lineNumber = 0;
    removeComments(pureCode);
    lbls = getLabels(pureCode);

    //Rule :
    //  1- Check that each line starts with an instruction
    //  2- Validates the operands of each instruction
    for (auto p : pureCode) {
        if(p.isEmpty()) continue; //for no reason, the loop goes to an empty string, this is here to fix the sigfault;
        ++lineNumber;
        p.remove(",");
        bool isLabel = p.endsWith(':');
        QStringList splittedLine = p.split(" ", QString::SkipEmptyParts);

        auto cmpLine = std::make_tuple(splittedLine.at(0).toUpper(),
                (splittedLine.count() >= 2 ? getOperandType(splittedLine.at(1)) : NOP),
                (splittedLine.count() >= 3 ? getOperandType(splittedLine.at(2)) : NOP));

        auto operandsResult = std::find(std::begin(operandsLUT), std::end(operandsLUT), cmpLine);

        auto instResult = std::find(std::begin(instructionsLUT),
                std::end(instructionsLUT),splittedLine.first().toUpper());

        QList<label>::iterator lblFindResult;
        qDebug() << Operands[getOperandType(splittedLine.at(1))];
        if((splittedLine.count() >= 2) && (getOperandType(splittedLine.at(1)) == LBL)) {
            qDebug() << "AA";
            QString buf = splittedLine.at(1);
            buf.remove(':');
            lblFindResult = std::find(std::begin(lbls), std::end(lbls), std::make_tuple(0, splittedLine.at(1), buf));
        }

        if(instResult == std::end(instructionsLUT) && !isLabel) {ret.append(std::make_tuple(QString("Unknown Instruction"), p, lineNumber)); continue;}
        if(operandsResult == std::end(operandsLUT) && !isLabel) {ret.append(std::make_tuple(QString("Invalid Operands"), p, lineNumber)); continue;}
        if(lblFindResult == std::end(lbls)) {ret.append(std::make_tuple(QString("Undefined Label"), p, lineNumber)); continue;}
    }
    return ret;
}
