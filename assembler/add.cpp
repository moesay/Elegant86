#include "base.h"
#include "add.h"

InstRet Add::process() {

    bool state = true;
    uchar modregrm;
    uchar mod = 0x00;
    QString machineCode;
    uchar opcode;
    uchar reg;

    try {
        threeTokens();
    }
    catch(const char* ex) {
        return {"", false, ex};
    }
    auto [mnemonic, firstOp, secondOp] = threeTokens();
    //get the operand type. is it a reg16, reg8, mem address, segreg or what?
    OperandType firstOpType = getOperandType(firstOp); OperandType secondOpType = getOperandType(secondOp);

    //A generic memory type is assigned to all memory addresses. in this section, based on the
    //other operand, the memory type is gonna be assigned
    if(firstOpType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? firstOpType = OperandType::Mem8 : firstOpType = OperandType::Mem16;
        else {
            if(secondOpType == OperandType::Reg16)
                firstOpType = OperandType::Mem16;
            else if(secondOpType == OperandType::Reg8)
                firstOpType = OperandType::Mem8;
            else
                return {"", false, "Unknown pointer size"};
        }
    }
    else if(secondOpType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? secondOpType = OperandType::Mem8 : secondOpType = OperandType::Mem16;
        else {
            if(firstOpType == OperandType::Reg16)
                secondOpType = OperandType::Mem16;
            else if(firstOpType == OperandType::Reg8)
                secondOpType = OperandType::Mem8;
            else
                return {"", false, "Unknown pointer size"};
        }
    }

    if(firstOpType == OperandType::Mem16 && secondOpType == OperandType::Immed8)
        secondOpType = Immed16;
    else if(secondOpType == OperandType::Mem16 && firstOpType == OperandType::Immed8)
        firstOpType = Immed16;

    //in the form of [firstOpType]-[secondOpType]
    QString generalExpression = Operands[firstOpType] + '-' + Operands[secondOpType];

    //if both the operands are of the same size
    if(((firstOpType==OperandType::Reg16) && (secondOpType==OperandType::Reg16)) ||
            ((firstOpType==OperandType::Reg8) && (secondOpType==OperandType::Reg8))) {
        //the mod is indeed 3, and the machine code is 2 bytes
        //the opcode, and the modregrm byte
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modRegRmGenerator(0x3, getGpRegCode(secondOp, secondOpType), getGpRegCode(firstOp, firstOpType))));
        return {machineCode, true, ""};
    }

    else if(firstOpType==OperandType::Reg8 && secondOpType==OperandType::Immed8) {
        if(firstOp == "AL") {
            opcode = getOpcode(firstOp+"-IMMED8", &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(hexToStr(opcode));
            machineCode.append(hexToStr(secondOp.toInt(nullptr, 16)));
            return {machineCode, true, ""};
        }

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        reg = 0X00;
        modregrm = modRegRmGenerator(0X3, reg, getGpRegCode(firstOp, firstOpType));
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modregrm));
        machineCode.append(hexToStr(secondOp.toInt(nullptr, 16)));
        return {machineCode, true, ""};
    }

    else if(firstOpType==OperandType::Reg16 && (secondOpType==OperandType::Immed16 || secondOpType==Immed8)){
        if(firstOp == "AX") {
            opcode = getOpcode(firstOp+"-IMMED16", &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(hexToStr(opcode));
            machineCode.append(hexToStr(secondOp.toInt(nullptr, 16), HexType::DirectAddress));
            return {machineCode, true, ""};
        }
        opcode = getOpcode(Operands[firstOpType]+"-IMMED16" , &state);
        if(state == false) return {"", false, "Invalid Operands"};
        reg = 0X00;
        machineCode.append(hexToStr(opcode)); //the instruction machine code, in the case its mov XX, XX is any 8 bit reg
        modregrm = modRegRmGenerator(0X3, reg, getGpRegCode(firstOp, firstOpType));
        machineCode.append(hexToStr(modregrm));
        machineCode.append(hexToStr(secondOp.toInt(nullptr, 16), HexType::DirectAddress));

        return {machineCode, true, ""};
    }

    else if(firstOpType==OperandType::Mem8 || secondOpType==OperandType::Mem8 ||
            firstOpType==OperandType::Mem16 || secondOpType==OperandType::Mem16) {

        bool destIsMemAddr = firstOpType == OperandType::Mem8 || firstOpType == OperandType::Mem16;
        QStringList addrArgs;

        if(destIsMemAddr) {
            firstOp.remove("["); firstOp.remove("]");
            addrArgs = firstOp.split(QRegExp("[+]"));
        } else {
            secondOp.remove("["); secondOp.remove("]");
            addrArgs = secondOp.split(QRegExp("[+]"));
        }

        QStringList displacment = addrArgs.filter(QRegularExpression("[0-9a-fA-F]"));
        if(std::any_of(std::begin(addrArgs), std::end(addrArgs), [](const QString& p) {
                    return (notAdressingRegs.contains(p) || Regs8.contains(p));
                    })) return {"", false, "Invalid addressing register/s"};
        hexValidator(displacment);

        bool directAddress = addrArgs.size() == 1;
        int displacmentValue = 0;

        if(displacment.size() >= 1)
            displacmentValue = displacment.first().toInt(0, 16);

        //Known Bug : The MOD for negative numbers is miscalculated
        auto Mod1 = [displacmentValue]()-> bool {
            return ((displacmentValue >= 0x0000 && displacmentValue <= 0x007F) ||
                    (displacmentValue >= 0xFF80 && displacmentValue < 0xFFFF));
        };

        if(displacment.empty()) mod = 0x00;         //for bx+si, bp+di ... and si, di as well
        else if(!displacment.empty()) {
            if(displacment.size() == 1 && addrArgs.size() == 1) mod = 0x00; //direct address;
            else mod = (Mod1() ? 0x01 : 0x02);       //the rest of the cases!
        }

        if((firstOp == "AL" || firstOp == "AX") && isHexValue(secondOp) && secondOpType != OperandType::Mem8 && secondOpType != Mem16) {
            uchar opcode = getOpcode(firstOp +'-'+ Operands[secondOpType], &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(hexToStr(opcode));
            machineCode.append(hexToStr(extractDisplacment(secondOp).toInt(0, 16), HexType::DirectAddress));
            return {machineCode, true, ""};
        }
        /* if((secondOp == "AL" || secondOp == "AX") && isHexValue(firstOp)) { */
        /*     qDebug() << Operands[firstOpType] << " " << secondOp; */
        /*     uchar opcode = getOpcode(Operands[firstOpType] +'-'+ secondOp, &state); */
        /*     if(state == false) return {"", false, "Invalid Operands"}; */
        /*     machineCode.append(hexToStr(opcode)); */
        /*     machineCode.append(hexToStr(extractDisplacment(firstOp).toInt(0, 16), HexType::DirectAddress)); */
        /*     return {machineCode, true, ""}; */
        /* } */

        if(destIsMemAddr) {
            if(secondOpType == OperandType::SegReg)
                reg = getSegRegCode(secondOp);
            if(secondOpType == OperandType::Reg16 || secondOpType == OperandType::Reg8)
                reg = getGpRegCode(secondOp, secondOpType);
            else if(secondOpType == Immed8 || secondOpType == Immed16)
                reg = 0X00;
            uchar rm = rmGenerator(firstOp);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        } else {
            if(firstOpType == OperandType::SegReg)
                reg = getSegRegCode(firstOp);
            else
                reg = getGpRegCode(firstOp, firstOpType);
            uchar rm = rmGenerator(secondOp);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        }

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modregrm));
        if(!displacment.empty())
            machineCode.append(hexToStr(displacment.first().toInt(0, 16), (directAddress ? HexType::DirectAddress : Address), Sign::Pos,
                        (mod == 0x01 ? OutputSize::Byte : OutputSize::Word)));
        if(secondOpType == OperandType::Immed8)
            machineCode.append(hexToStr(secondOp.toInt(nullptr, 16)));
        else if (secondOpType == OperandType::Immed16)
            machineCode.append(hexToStr(secondOp.toInt(nullptr, 16), HexType::OpCode, Sign::Pos, OutputSize::Word));
        return {machineCode, true, ""};
    }

    else if(firstOpType==OperandType::Reg16 && secondOpType==OperandType::SegReg) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(secondOp), getGpRegCode(firstOp, firstOpType));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(firstOpType==OperandType::SegReg && secondOpType==OperandType::Reg16) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(firstOp), getGpRegCode(secondOp, secondOpType));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return {machineCode, true, ""};
    }

    return {"", false, "Unhandled Error"};
}

uchar Add::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toUpper().toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; //not used
}
