#include "base.h"
#include "mov.h"

QString Mov::process() {

    uchar modregrm;
    uchar mod = 0x00;
    QString machineCode;
    uchar opcode;

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
        machineCode.append(hexToStr(modRegRmGenerator(0x3, getGpRegCode(secondOp), getGpRegCode(firstOp))));
        return machineCode;
    }

    else if(firstOpType==OperandType::Reg8 && secondOpType==OperandType::Immed8) {
        opcode = getOpcode(firstOp+"-IMMED8");
        machineCode.append(hexToStr(opcode));
        machineCode.append(secondOp);
        return machineCode;
    }

    else if(firstOpType==OperandType::Reg16 && (secondOpType==OperandType::Immed16 || secondOpType==Immed8)){
        opcode = getOpcode(firstOp.toUpper()+"-IMMED16");
        machineCode.append(hexToStr(opcode)); //the instruction machine code, in the case its mov XX, XX is any 8 bit reg
        machineCode.append(hexToStr(secondOp.toInt(nullptr, 16), HexType::DirectAddress));

        return machineCode;
    }

    else if(firstOpType==OperandType::MemAddr || secondOpType==OperandType::MemAddr) {
        bool destIsMemAddr = firstOpType==OperandType::MemAddr;
        QStringList addrArgs;
        uchar reg;

        if(destIsMemAddr) {
            firstOp.remove("["); firstOp.remove("]");
            addrArgs = firstOp.split(QRegExp("[+-]"));
        } else {
            secondOp.remove("["); secondOp.remove("]");
            addrArgs = secondOp.split(QRegExp("[+-]"));
        }

        QStringList displacment = addrArgs.filter(QRegularExpression("[0-9a-fA-F]"));
        hexValidator(displacment);

        bool directAddress = addrArgs.size() == 1;

        int displacmentValue = 0;
        if(displacment.size() >= 1)
            displacmentValue = displacment.first().toInt(0, 16);

        auto Mod1 = [displacmentValue]()-> bool {
            return ((displacmentValue >= 0x0000 && displacmentValue <= 0x007F) ||
                    (displacmentValue >= 0xFF80 && displacmentValue <= 0xFFFF));
        };

        if(displacment.empty()) mod = 0x00;         //for bx+si, bp+di ... and si, di as well
        else if(!displacment.empty()) {
            if(displacment.size() == 1 && addrArgs.size() == 1) mod = 0x00; //direct address;
            else mod = (Mod1() ? 0x01 : 0x02);       //the rest of the cases!
        }

        if (firstOp == "AL" && isHexValue(secondOp)) { machineCode.append(hexToStr(getOpcode(firstOp+"-MEM")));
            machineCode.append(hexToStr(extractDisplacment(secondOp).toInt(0, 16), HexType::DirectAddress)); return machineCode;}
        if (firstOp == "AX" && isHexValue(secondOp)) { machineCode.append(hexToStr(getOpcode(firstOp+"-MEM")));
            machineCode.append(hexToStr(extractDisplacment(secondOp).toInt(0, 16), HexType::DirectAddress)); return machineCode;}

        if(secondOp == "AL" && isHexValue(firstOp)) { machineCode.append(hexToStr(getOpcode("MEM-"+secondOp)));
            machineCode.append(hexToStr(extractDisplacment(firstOp).toInt(0, 16), HexType::DirectAddress)); return machineCode;}
        if(secondOp == "AX" && isHexValue(firstOp)) { machineCode.append(hexToStr(getOpcode("MEM-"+secondOp)));
            machineCode.append(hexToStr(extractDisplacment(firstOp).toInt(0, 16), HexType::DirectAddress)); return machineCode;}
        //---

        if(destIsMemAddr) {
            if(secondOpType == OperandType::SegReg)
                reg = getSegRegCode(secondOp);
            else
                reg = getGpRegCode(secondOp);
            modregrm = modRegRmGenerator(mod, reg, rmGenerator(firstOp));
        } else {
            if(firstOpType == OperandType::SegReg)
                reg = getSegRegCode(firstOp);
            else
                reg = getGpRegCode(firstOp);
            modregrm = modRegRmGenerator(mod, reg, rmGenerator(secondOp));
        }

        opcode = getOpcode(generalExpression);
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modregrm));
        if(!displacment.empty())
            machineCode.append(hexToStr(displacment.first().toInt(0, 16), (directAddress ? HexType::DirectAddress : Address)));
        return machineCode;
    }

    else if(firstOpType==OperandType::Reg16 && secondOpType==OperandType::SegReg) {
        opcode = getOpcode(generalExpression);
        modregrm = modRegRmGenerator(0x03, getSegRegCode(secondOp), getGpRegCode(firstOp));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return machineCode;
    }

    else if(firstOpType==OperandType::SegReg && secondOpType==OperandType::Reg16) {
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

Mov::~Mov(){}
Mov::Mov(){}
