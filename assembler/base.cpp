#include "base.h"

QList<label> lbls;

Base::~Base(){}

bool Base::isMemAddr(const QString &param) {
    if(param.startsWith('[') && param.endsWith(']'))
        return true;
    return false;
}

std::tuple<QString, QString> Base::twoTokens() {
    QStringList list = codeLine.split(QRegExp(" "), QString::SkipEmptyParts);
    return {list.at(0), list.at(1)};
}

//tested
std::tuple<QString, QString, QString> Base::threeTokens() {
    QStringList list = codeLine.split(QRegExp(" |\\,"), QString::SkipEmptyParts);
    return {list[0].toUpper(), list[1].toUpper(), list[2].toUpper()};
}

//tested
enum OperandType Base::getOperandType(const QString& operand) {

    /* qDebug() << "HERE : " << std::get<1>(lbls.first()) << std::get<2>(lbls.first()); */
    /* qDebug() << "THERE : " << operand; */
    if(Regs8.contains(operand.trimmed().toUpper())) return OperandType::Reg8;
    else if(Regs16.contains(operand.trimmed().toUpper())) return OperandType::Reg16;
    else if(SegRegs.contains(operand.trimmed().toUpper())) return OperandType::SegReg;
    else if(isMemAddr(operand)) return OperandType::MemAddr;
    else if(std::find(std::begin(lbls), std::end(lbls), std::make_tuple(0, operand,
                    const_cast<QString*>(&operand)->remove(':'))) != std::end(lbls)) return OperandType::LBL;
    else if(isImmed8(operand)) return (operand.toInt(nullptr, 16) >= 0 ? OperandType::Immed8 : OperandType::NegImmed8);
    else if(isImmed16(operand)) return (operand.toInt(nullptr, 16) >= 0? OperandType::Immed16 : OperandType::NegImmed16);
    return OperandType::Unknown;
}

uchar Base::rmGenerator(const QString& param) {
    QString addr = param.trimmed().toUpper();

    QStringList argList = addr.split(QRegExp("[+-]"), QString::SkipEmptyParts);
    if(argList.size() >= 2) {
        if(argList.contains("BX") && argList.contains("SI")) return mod00["BX+SI"];
        else if(argList.contains("BX") && argList.contains("DI")) return mod00["BX+DI"];
        else if(argList.contains("BP") && argList.contains("SI")) return mod00["BP+SI"];
        else if(argList.contains("BP") && argList.contains("DI")) return mod00["BP+DI"];
        else if(argList.contains("BX") && !argList.contains("SI") && !argList.contains("DI")) return mod00["BX"];
        else if(argList.contains("DI") && !argList.contains("BX") && !argList.contains("BP")) return mod00["DI"];
        else if(argList.contains("SI") && !argList.contains("BX") && !argList.contains("BP")) return mod00["SI"];
    }
    else if(argList.size() == 1) {
        if(argList.contains("BX")) return mod00["BX"];
        else if(argList.contains("SI")) return mod00["SI"];
        else if(argList.contains("DI")) return mod00["DI"];
        else return mod00["DA"];
    }
    //on error
    return 0xff;
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

uchar Base::getGpRegCode(const QString& param, bool *ok) {
    auto match = gpRegsHex.find(param.toUpper().toStdString());
    if(match != std::end(gpRegsHex)) {
        if(ok != nullptr) *ok = false;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

QString Base::extractDisplacment(const QString& param, bool *ok) {
    QStringList splittedAddress = param.split(QRegExp("[+-]"));
    QStringList filteredDisplacment = splittedAddress.filter(QRegularExpression("[0-9a-fA-F]"));

    if(filteredDisplacment.size() > 0) {
        filteredDisplacment.first().remove(']'); filteredDisplacment.first().remove('[');
        if(ok != nullptr) *ok=true;
        return filteredDisplacment.first();
    }
    if(ok != nullptr) *ok = false;
    return "";
}

uchar Base::getSegRegCode(const QString& param, bool *ok) {
    auto match = segRegsHex.find(param.toUpper().toStdString());
    if(match != std::end(segRegsHex)) {
        if(ok != nullptr) *ok = false;
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

QString Base::signHandler(QString&& param, const OperandType& ot) {
    if(ot == OperandType::NegImmed8) {
        uint8_t hexVal = param.toInt(nullptr, 16);
        return QString::number(hexVal, 16).toUpper();
    }
    else if(ot == OperandType::NegImmed16) {
        uint16_t hexVal = param.toInt(nullptr, 16);
        return QString::number(hexVal, 16).toUpper();
    }
    return "";
}

Base::Base(){}
