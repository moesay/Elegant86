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
    auto [mnemonic, dest, src] = threeTokens();

    try {
    segmentPrefixWrapper(dest, src, machineCode);
    } catch(InvalidSegmentOverridePrefix& exc) {
        return {"", false, exc.what()};
    }

    //get the operand type. is it a reg16, reg8, mem address, segreg or what?
    OperandType destType = getOperandType(dest); OperandType srcType = getOperandType(src);

    //A generic memory type is assigned to all memory addresses. in this section, based on the
    //other operand, the memory type is gonna be assigned
    if(destType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? destType = OperandType::Mem8 : destType = OperandType::Mem16;
        else {
            if(srcType == OperandType::Reg16)
                destType = OperandType::Mem16;
            else if(srcType == OperandType::Reg8)
                destType = OperandType::Mem8;
            else
                return {"", false, "Unknown pointer size"};
        }
    }
    else if(srcType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? srcType = OperandType::Mem8 : srcType = OperandType::Mem16;
        else {
            if(destType == OperandType::Reg16)
                srcType = OperandType::Mem16;
            else if(destType == OperandType::Reg8)
                srcType = OperandType::Mem8;
            else
                return {"", false, "Unknown pointer size"};
        }
    }

    if(destType == OperandType::Mem16 && srcType == OperandType::Immed8)
        srcType = Immed16;
    else if(srcType == OperandType::Mem16 && destType == OperandType::Immed8)
        destType = Immed16;

    //in the form of [destType]-[srcType]
    QString generalExpression = Operands[destType] + '-' + Operands[srcType];

    //if both the operands are of the same size
    if(((destType==OperandType::Reg16) && (srcType==OperandType::Reg16)) ||
            ((destType==OperandType::Reg8) && (srcType==OperandType::Reg8))) {
        //the mod is indeed 3, and the machine code is 2 bytes
        //the opcode, and the modregrm byte
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modRegRmGenerator(0x3, getGpRegCode(src, srcType), getGpRegCode(dest, destType))));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg8 && srcType==OperandType::Immed8) {
        if(dest == "AL") {
            opcode = getOpcode(dest+"-IMMED8", &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(hexToStr(opcode));
            machineCode.append(hexToStr(src.toInt(nullptr, 16)));
            return {machineCode, true, ""};
        }

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        reg = 0X00;
        modregrm = modRegRmGenerator(0X3, reg, getGpRegCode(dest, destType));
        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modregrm));
        machineCode.append(hexToStr(src.toInt(nullptr, 16)));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg16 && (srcType==OperandType::Immed16 || srcType==Immed8)){
        if(dest == "AX") {
            opcode = getOpcode(dest+"-IMMED16", &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(hexToStr(opcode));
            machineCode.append(hexToStr(src.toInt(nullptr, 16), HexType::DirectAddress));
            return {machineCode, true, ""};
        }
        opcode = getOpcode(Operands[destType]+"-IMMED16" , &state);
        if(state == false) return {"", false, "Invalid Operands"};
        reg = 0X00;
        machineCode.append(hexToStr(opcode)); //the instruction machine code, in the case its mov XX, XX is any 8 bit reg
        modregrm = modRegRmGenerator(0X3, reg, getGpRegCode(dest, destType));
        machineCode.append(hexToStr(modregrm));
        machineCode.append(hexToStr(src.toInt(nullptr, 16), HexType::DirectAddress));

        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Mem8 || srcType==OperandType::Mem8 ||
            destType==OperandType::Mem16 || srcType==OperandType::Mem16) {

        bool destIsMemAddr = destType == OperandType::Mem8 || destType == OperandType::Mem16;
        QStringList addrArgs;

        if(destIsMemAddr) {
            dest.remove("["); dest.remove("]");
            addrArgs = dest.split(QRegExp("[+]"));
        } else {
            src.remove("["); src.remove("]");
            addrArgs = src.split(QRegExp("[+]"));
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

        if((dest == "AL" || dest == "AX") && isHexValue(src) && srcType != OperandType::Mem8 && srcType != Mem16) {
            uchar opcode = getOpcode(dest +'-'+ Operands[srcType], &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(hexToStr(opcode));
            machineCode.append(hexToStr(extractDisplacment(src).toInt(0, 16), HexType::DirectAddress));
            return {machineCode, true, ""};
        }

        if(destIsMemAddr) {
            if(srcType == OperandType::SegReg)
                reg = getSegRegCode(src);
            if(srcType == OperandType::Reg16 || srcType == OperandType::Reg8)
                reg = getGpRegCode(src, srcType);
            else if(srcType == Immed8 || srcType == Immed16)
                reg = 0X00;
            uchar rm = rmGenerator(dest);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        } else {
            if(destType == OperandType::SegReg)
                reg = getSegRegCode(dest);
            else
                reg = getGpRegCode(dest, destType);
            uchar rm = rmGenerator(src);
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
        if(srcType == OperandType::Immed8)
            machineCode.append(hexToStr(src.toInt(nullptr, 16)));
        else if (srcType == OperandType::Immed16)
            machineCode.append(hexToStr(src.toInt(nullptr, 16), HexType::OpCode, Sign::Pos, OutputSize::Word));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg16 && srcType==OperandType::SegReg) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(src), getGpRegCode(dest, destType));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::SegReg && srcType==OperandType::Reg16) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(dest), getGpRegCode(src, srcType));
        machineCode.append(hexToStr(opcode)); machineCode.append(hexToStr(modregrm));
        return {machineCode, true, ""};
    }

    return {"", false, "Invalid Operands"};
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

Add::Add(const QString& param) {
    this->setCodeLine(param);
}
Add::Add() {}
