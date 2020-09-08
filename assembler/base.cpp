#include "base.h"

QList<label> lbls;

bool Base::isMemAddr(const QString &param) {
    if(param.startsWith('[') && param.endsWith(']'))
        return true;
    return false;
}

std::tuple<QString, QString> Base::twoTokens() {
    pointerType = Pointer::None;
    codeLine.replace("WPTR", "WPTR ", Qt::CaseSensitivity::CaseInsensitive);
    codeLine.replace("BPTR", "BPTR ", Qt::CaseSensitivity::CaseInsensitive);

    QStringList list = codeLine.split(QRegExp(" "), QString::SkipEmptyParts);
    for(int i = 0; i < list.length(); i++) {
        if(list[i].toUpper() == "WPTR") {
            if(getOperandType(list.at(i+1)) == OperandType::Immed8 || getOperandType(list[i+1]) == OperandType::Immed16)
                throw InvalidPointer();
            pointerType = Pointer::Word;
            list.removeAt(i);
        }
        else if(list[i].toUpper() == "BPTR") {
            if(getOperandType(list.at(i+1)) == OperandType::Immed8 || getOperandType(list[i+1]) == OperandType::Immed16)
                throw InvalidPointer();
            pointerType = Pointer::Byte;
            list.removeAt(i);
        }
    }
    assert(list.count() >= 2);
    return {list.at(0), list.at(1)};
}

//A seg fault when a wptr is found at the end of the line, fix it later.
std::tuple<QString, QString, QString> Base::threeTokens() {
    pointerType = Pointer::None;
    codeLine.replace("WPTR", "WPTR ", Qt::CaseSensitivity::CaseInsensitive);
    codeLine.replace("BPTR", "BPTR ", Qt::CaseSensitivity::CaseInsensitive);
    QStringList list = codeLine.split(QRegExp(" |\\,"), QString::SkipEmptyParts);
    for(int i = 0; i < list.length(); i++) {
        if(list[i].toUpper() == "WPTR") {
            if(getOperandType(list.at(i+1)) == OperandType::Immed8 || getOperandType(list[i+1]) == OperandType::Immed16)
                throw InvalidPointer();
            pointerType = Pointer::Word;
            list.removeAt(i);
        }
        else if(list[i].toUpper() == "BPTR") {
            if(getOperandType(list.at(i+1)) == OperandType::Immed8 || getOperandType(list[i+1]) == OperandType::Immed16)
                throw InvalidPointer();
            pointerType = Pointer::Byte;
            list.removeAt(i);
        }
    }
    assert(list.count() >= 3);
    return {list[0].toUpper(), list[1].toUpper(), list[2].toUpper()};
}

bool Base::hasSegmentPrefix(const QString& param) {
    for(const auto &p : SegRegs) {
        if(param.toUpper().startsWith(p+':'))
            return true;
    }
    return false;
}

QString Base::getSegmentPrefix(const QString& param) {
    return param.toUpper().split(':').at(0);
}

QString Base::stripSegmentPrefix(const QString& param) {
    if(hasSegmentPrefix(param))
        return param.split(':').at(1);
    return param;
}

void Base::segmentPrefixWrapper(QString& firstOp, QString& machineCode) {
    if(hasSegmentPrefix(firstOp)) {
        machineCode.append(hexToStr(getSegRegPrefix(getSegmentPrefix(firstOp))));
        firstOp = stripSegmentPrefix(firstOp);
    }
}

void Base::segmentPrefixWrapper(QString& firstOp, QString& secondOp, QString& machineCode) {
    if(hasSegmentPrefix(firstOp) && hasSegmentPrefix(secondOp))
        throw InvalidSegmentOverridePrefix();
    else if(hasSegmentPrefix(firstOp)) {
        machineCode.append(hexToStr(getSegRegPrefix(getSegmentPrefix(firstOp))));
        firstOp = stripSegmentPrefix(firstOp);
    }
    else if(hasSegmentPrefix(secondOp)) {
        machineCode.append(hexToStr(getSegRegPrefix(getSegmentPrefix(secondOp))));
        secondOp = stripSegmentPrefix(secondOp);
    }
}

enum OperandType Base::getOperandType(const QString& operand) {
    QString strippedOperand = stripSegmentPrefix(operand);
    if(isChar(strippedOperand)) return OperandType::Char;
    else if(Regs8.contains(strippedOperand.trimmed().toUpper())) return OperandType::Reg8;
    else if(Indexers.contains(strippedOperand.trimmed().toUpper())) return OperandType::Indexer;
    else if(Regs16.contains(strippedOperand.trimmed().toUpper())) return OperandType::Reg16;
    else if(SegRegs.contains(strippedOperand.trimmed().toUpper())) return OperandType::SegReg;
    else if(isMemAddr(strippedOperand)) return OperandType::MemAddr;
    //TBF, remove the const cast and add buf variable.
    else if(std::find(std::begin(lbls), std::end(lbls), std::make_tuple(0,
                    const_cast<QString*>(&strippedOperand)->remove(':'))) != std::end(lbls)) return OperandType::Label;
    else if(isImmed8(strippedOperand)) return (strippedOperand.toInt(nullptr, 16) >= 0 ? OperandType::Immed8 : OperandType::NegImmed8);
    else if(isImmed16(strippedOperand)) return (strippedOperand.toInt(nullptr, 16) >= 0? OperandType::Immed16 : OperandType::NegImmed16);
    return OperandType::Unknown;
}

uchar Base::rmGenerator(const QString& param) {
    QString addr = param.trimmed().toUpper();

    QStringList argList = addr.split(QRegExp("[+]"), QString::SkipEmptyParts);
    if(argList.size() >= 2) {
        if(argList.contains("BX") && argList.contains("SI")) return mod00["BX+SI"];
        else if(argList.contains("BX") && argList.contains("DI")) return mod00["BX+DI"];
        else if(argList.contains("BP") && argList.contains("SI")) return mod00["BP+SI"];
        else if(argList.contains("BP") && argList.contains("DI")) return mod00["BP+DI"];
        else if(argList.contains("BX") && std::none_of(std::begin(addressingRegs), std::end(addressingRegs),
                    [argList](const QString& p) {return argList.contains(p) && p != "BX";})) return mod00["BX"];
        else if(argList.contains("DI") && std::none_of(std::begin(addressingRegs), std::end(addressingRegs),
                    [argList](const QString& p) {return argList.contains(p) && p != "DI";})) return mod00["DI"];
        else if(argList.contains("SI") && std::none_of(std::begin(addressingRegs), std::end(addressingRegs),
                    [argList](const QString& p) {return argList.contains(p) && p != "SI";})) return mod00["SI"];
    }
    else if(argList.size() == 1) {
        if(argList.contains("BX")) return mod00["BX"];
        else if(argList.contains("SI")) return mod00["SI"];
        else if(argList.contains("DI")) return mod00["DI"];
        else return mod00["DA"];
    }
    //on error
    return 0xFF;
}

bool Base::isChar(const QString& param) {
    return ((param.length() == 3) &&
            (param.startsWith("'")) &&
            (param.endsWith("'")));
}

bool Base::isImmed8(const QString& param) {
    return isHexValue(param) && abs(param.toInt(nullptr, 16)) <= 0xFF;
}

bool Base::isImmed16(const QString& param) {
    return isHexValue(param) && (abs(param.toInt(nullptr, 16)) > 0xFF && abs(param.toInt(nullptr, 16)) <= 0xFFFF);
}

void Base::setCodeLine(const QString& param) {
    codeLine = param;
}

const QString& Base::getCodeLine() { return codeLine; }

uchar Base::modRegRmGenerator(const uchar& mod, const uchar& reg, const uchar& rm) {
    return ((mod << 6) | (reg << 3) | (rm));
}

uchar Base::getGpRegCode(const QString& param, OperandType ot, bool *ok) {
    std::unordered_map<std::string, uchar>::iterator match;
    if(ot == OperandType::Reg16)
        match = Regs16Hex.find(param.toUpper().toStdString());
    else if(ot == OperandType::Reg8)
        match = Regs8Hex.find(param.toUpper().toStdString());
    if(match != std::end(Regs16Hex)) {
        if(ok != nullptr) *ok = false;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

QString Base::extractDisplacment(const QString& param, bool *ok) {
    QStringList splittedAddress = param.split(QRegExp("[+]"));
    QStringList filteredDisplacment = splittedAddress.filter(QRegularExpression("[0-9a-fA-F]"));

    if(filteredDisplacment.size() > 0) {
        filteredDisplacment.first().remove(']'); filteredDisplacment.first().remove('[');
        if(ok != nullptr) *ok=true;
        return filteredDisplacment.first();
    }
    if(ok != nullptr) *ok = false;
    return "";
}

uchar Base::getSegRegPrefix(const QString& param, bool *ok) {
    auto match = segRegsPrefix.find(param.toUpper().toStdString());
    if(match != std::end(segRegsHex)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

uchar Base::getSegRegCode(const QString& param, bool *ok) {
    auto match = segRegsHex.find(param.toUpper().toStdString());
    if(match != std::end(segRegsHex)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

bool Base::isHexValue(const QString& param) {
    bool b;
    param.toInt(&b, 16);
    return b;
}

void Base::hexValidator(QStringList& param) {
    param.erase(
            std::remove_if(
                std::begin(param),
                std::end(param),
                [this](QString const &p) {return !this->isHexValue(p);}), std::end(param));
}

QString Base::signHandler(const QString& param, const OperandType& ot) {
    if(ot == OperandType::NegImmed8) {
        if(abs(param.toInt(nullptr, 16)) > 0xFE) {
            uint16_t hexVal = param.toInt(nullptr, 16);
            return hexToStr(hexVal, HexType::OpCode, Sign::Neg);
            /* return QString::number(hexVal, 16).toUpper(); */
        }
        else {
            uint8_t hexVal = param.toInt(nullptr, 16);
            return hexToStr(hexVal, HexType::OpCode, Sign::Neg);
            /* return QString::number(hexVal, 16).toUpper(); */
        }
    }
    else if(ot == OperandType::NegImmed16) {
        if(abs(param.toInt(nullptr, 16)) > 0x7FFF) return "ERROR";
        uint16_t hexVal = param.toInt(nullptr, 16);
        return hexToStr(hexVal, HexType::OpCode, Sign::Neg);
        /* return QString::number(hexVal, 16).toUpper(); */
    }
    return "ERROR";
}
