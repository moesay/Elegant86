#include "parser.h"

Instruction::~Instruction(){}

//tested
bool Instruction::isMemAddr(const QString &param) {
    if(param.startsWith('[') && param.endsWith(']'))
        return true;
    return false;
}

std::tuple<QString, QString> Instruction::twoTokens() {
    QStringList list = codeLine.split(QRegExp(" "), QString::SkipEmptyParts);
    return {list.at(0), list.at(1)};
}

//tested
std::tuple<QString, QString, QString> Instruction::threeTokens() {
    QStringList list = codeLine.split(QRegExp(" |\\,"), QString::SkipEmptyParts);
    return {list[0].toUpper(), list[1].toUpper(), list[2].toUpper()};
}

//tested
enum OperandType Instruction::getOperandType(const QString& operand) {
    if(Regs8.contains(operand.trimmed().toUpper())) return OperandType::Reg8;
    if(Regs16.contains(operand.trimmed().toUpper())) return OperandType::Reg16;
    if(SegRegs.contains(operand.trimmed().toUpper())) return OperandType::segReg;
    if(isMemAddr(operand)) return OperandType::Mem16;
    if(isImmed8(operand)) return OperandType::Immed8;
    if(isImmed16(operand)) return OperandType::Immed16;
    return OperandType::Unknown;
}

uchar Instruction::rmGenerator(const QString& param) {
    QString addr = param.trimmed().toUpper();

    QStringList argList = addr.split("+", QString::SkipEmptyParts);
    if(argList.size() >= 2) {
        if(argList.contains("BX") && argList.contains("SI")) return mod00["BX+SI"];
        else if(argList.contains("BX") && argList.contains("DI")) return mod00["BX+DI"];
        else if(argList.contains("BP") && argList.contains("SI")) return mod00["BP+SI"];
        else if(argList.contains("BP") && argList.contains("DI")) return mod00["BP+DI"];
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

//TODO : make a numirical check insted of checking by th
//string length
bool Instruction::isImmed8(const QString& param) {
    return (param.length() <= 2);
}

//TODO : make a numirical check insted of checking by th
//string length
bool Instruction::isImmed16(const QString& param) {
    return (param.length() > 2 && param.length() <= 4);
}

void Instruction::setCodeLine(const QString& param) {
    codeLine = param;
}

const QString& Instruction::getCodeLine() { return codeLine; }

uchar Instruction::modRegRmGenerator(const uchar& mod, const uchar& reg, const uchar& rm) {
    return ((mod << 6) | (reg << 3) | (rm));
}

/*
 * ERROR DISCREPTION :
 *      WHEN PASSING A 4 DIGITS HEX NUMBER TO HEXTOSTR, IT TURNCATES IT INTO
 *      2 DIGITS ONLY BECAUSE ITS ARGUMENT IS UCHAR, FF MAXIMUM LENGTH
 *
 *      POSSIBLE SOLUTION : IMPLEMENT A DISPLACMENT MANAGER
 */
QString Mov::process() {
    uchar modregrm;
    uchar mod = 0x00;
    QString machineCode;
    uchar opcode;
    Displacment displacment;
    //split the instruction into
    auto [mnemonic, firstOp, secondOp] = threeTokens();
    //get the operand type. is it a reg16, reg8, mem address, segreg or what?
    OperandType firstOpType = getOperandType(firstOp); OperandType secondOpType = getOperandType(secondOp);
    //in the form of [firstOpType]-[secondOpType]
    QString generalExpression = Operands[firstOpType] + '-' + Operands[secondOpType];

    //if both the operands are of the same size
    if(((firstOpType==OperandType::Reg16) && (secondOpType==OperandType::Reg16)) ||
            ((firstOpType==OperandType::Reg8) && (secondOpType==OperandType::Reg8))) {
        //the mod is indeed 3, and the machine code is 2 bytes
        //the opcode, and the modregrm byte
        opcode = getOpcode(generalExpression);
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modRegRmGenerator(0x3, getGpRegCode(firstOp), getGpRegCode(secondOp))));
        return machineCode;
    }

    //if the first operand is reg8 and the second is immed8
    else if(generalExpression=="REG8-IMMED8") {
        //build an expression that holds that reg name
        //because each 8bit reg has its own instruction
        opcode = getOpcode(firstOp+"-IMMED8");
        machineCode.append(hexToStr(opcode));
        machineCode.append(secondOp);
        return machineCode;
    }

    else if(generalExpression=="REG16-IMMED16" || generalExpression=="REG16-IMMED8") {
        opcode = getOpcode(firstOp.toUpper()+"-IMMED16");
        machineCode.append(hexToStr(opcode)); //the instruction machine code, in the case its mov XX, XX is any 8 bit reg
        machineCode.append(secondOp);

        return machineCode;
    }

    else if(generalExpression.startsWith("MEM") || generalExpression.endsWith("MEM")) {
        bool destIsMemAddr = generalExpression.startsWith("MEM");
        QStringList AddrArgs;
        uchar reg;

        if (firstOp == "AL" && !secondOp.contains('+')) { machineCode.append(hexToStr(getOpcode(firstOp+"-MEM")));
            machineCode.append(extractDisplacment(secondOp)); return machineCode;}
        if (firstOp == "AX" && !secondOp.contains('+')) { machineCode.append(hexToStr(getOpcode(firstOp+"-MEM")));
            machineCode.append(extractDisplacment(secondOp)); return machineCode;}

        if(secondOp == "AL" && !firstOp.contains('+')) { machineCode.append(hexToStr(getOpcode("MEM-"+secondOp)));
            machineCode.append(extractDisplacment(firstOp)); return machineCode;}
        if(secondOp == "AX" && !firstOp.contains('+')) { machineCode.append(hexToStr(getOpcode("MEM-"+secondOp)));
            machineCode.append(extractDisplacment(firstOp)); return machineCode;}

        if(destIsMemAddr) {
            firstOp.remove("["); firstOp.remove("]");
            AddrArgs = firstOp.split("+");
        } else {
            secondOp.remove("["); secondOp.remove("]");
            AddrArgs = secondOp.split("+");
        }
        QStringList displacment = AddrArgs.filter(QRegularExpression("0[xX][0-9a-fA-F]"));

        if(!displacment.empty()) {
            if(displacment.size() == 0 && AddrArgs.size() == 2) {mod = 0x00;}
            if(displacment.size() == 1 && AddrArgs.size() == 1) {mod = 0x00;}
            if(displacment.size() == 0 && AddrArgs.size() == 1) {mod = 0x00;}
            else if (displacment.first().toInt(0, 16) <= 65407 && AddrArgs.size() > 1) mod = 0x02;
            else if (displacment.first().toInt(0, 16) >= 65423 && AddrArgs.size() > 1) {
                //change the mod to 0x01 and trim the displacment as we already know, its 16 bit dsip.
                mod = 0x01;
                int displacmentMinusFF = displacment.first().toInt(0, 16) - 0xFF00;
                displacment.first() = hexToStr(displacmentMinusFF);
            }
        }
        if(destIsMemAddr) {
            if(secondOpType == OperandType::segReg)
                reg = getSegRegCode(secondOp);
            else
                reg = getGpRegCode(secondOp);
            modregrm = modRegRmGenerator(mod, reg, rmGenerator(firstOp));
        } else {
            if(firstOpType == OperandType::segReg)
                reg = getSegRegCode(firstOp);
            else
                reg = getGpRegCode(firstOp);
            modregrm = modRegRmGenerator(mod, reg, rmGenerator(secondOp));
        }

        opcode = getOpcode(generalExpression);
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modregrm));
        if(!displacment.empty())
            machineCode.append(hexToStr(displacment.first().toInt(0, 16)));
        return machineCode;
    }

    else if (generalExpression == "REG16-SEGREG") {
        opcode = getOpcode(generalExpression);
        modregrm = modRegRmGenerator(0x03, getSegRegCode(secondOp), getGpRegCode(firstOp));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return machineCode;
    }

    else if (generalExpression == "SEGREG-REG16") {
        opcode = getOpcode(generalExpression);
        modregrm = modRegRmGenerator(0x03, getSegRegCode(firstOp), getGpRegCode(secondOp));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return machineCode;
    }

    return machineCode;
}

uchar Mov::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toUpper().toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x0;
}

uchar Instruction::getGpRegCode(const QString& param, bool *ok) {
    auto match = gpRegsHex.find(param.toUpper().toStdString());
    if(match != std::end(gpRegsHex)) {
        if(ok != nullptr) *ok = false;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

QString Instruction::extractDisplacment(const QString& param, bool *ok) {
    QStringList splittedAddress = param.split('+');
    QStringList filteredDisplacment = splittedAddress.filter(QRegularExpression("0[xX][0-9a-fA-F]"));

    if(filteredDisplacment.size() > 0) {
        filteredDisplacment.first().remove(']'); filteredDisplacment.first().remove('[');
        if(filteredDisplacment.first().toLower().startsWith("0x")) {
            filteredDisplacment.first().remove(0, 2); }
        if(ok != nullptr) *ok=true;
        return filteredDisplacment.first();
    }
    if(ok != nullptr) *ok = false;
    return "";
}

uchar Instruction::getSegRegCode(const QString& param, bool *ok) {
    auto match = segRegsHex.find(param.toUpper().toStdString());
    if(match != std::end(segRegsHex)) {
        if(ok != nullptr) *ok = false;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

Mov::~Mov(){}
