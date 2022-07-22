#include <assembler/include/base.h>
#include <assembler/include/and.h>

InstRet_T And::process() {
    machineCode.clear();

    std::optional<std::tuple<QString, QString, QString>> temp = tokenize();

    if(temp == std::nullopt)
        return {"", false, "Invalid Pointer"};
    auto [mnemonic, dest, src] = *temp;

    try {
        segmentPrefixWrapper(dest, src);
    } catch(InvalidSegmentOverridePrefix& exc) {
        return {"", false, exc.what()};
    }

    destType = getOperandType(dest); srcType = getOperandType(src);

    if(destType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? destType = OperandType::Mem8 : destType = OperandType::Mem16;
        else {
            if(srcType == OperandType::Reg16 || srcType == OperandType::SegReg)
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
            if(destType == OperandType::Reg16 || destType == OperandType::SegReg || destType == OperandType::Indexer) //add the indexer to the rest
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
    if(srcType == OperandType::Immed16 && destType == OperandType::Mem8)
        srcType = OperandType::Immed8;

    QString generalExpression = Operands[destType] + '-' + Operands[srcType];

    if(((destType==OperandType::Reg16) && (srcType==OperandType::Reg16)) ||
            ((destType==OperandType::Reg8) && (srcType==OperandType::Reg8))) {

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));

        machineCode.append(numToHexStr(modRegRmGenerator(0X3, getGpRegCode(src, srcType), getGpRegCode(dest, destType))));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg8 && srcType==OperandType::Immed8) {
        if(dest=="AL") {
            opcode = getOpcode("AL-IMMED8", &state);
            machineCode.append(numToHexStr(opcode));
            machineCode.append(numToHexStr(src));
            return {machineCode, true, ""};
        }
        else {
            opcode = getOpcode(generalExpression, &state);
            if(state == false) return {"", false, "Invalid Operands"};

            machineCode.append(numToHexStr(opcode));
            //and reg8/immed8 has a fixed reg field
            machineCode.append(numToHexStr(modRegRmGenerator(0X3, 0X4, getGpRegCode(dest, destType))));
            machineCode.append(numToHexStr(src));
            return {machineCode, true, ""};
        }
    }

    else if(destType==OperandType::Reg16 && (srcType==OperandType::Immed16 || srcType==Immed8)){
        if(dest=="AX") {
            opcode = getOpcode("AX-IMMED16", &state);
            machineCode.append(numToHexStr(opcode));
            machineCode.append(numToHexStr(src));
            return {machineCode, true, ""};
        }
        else {
            opcode = getOpcode(generalExpression, &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(numToHexStr(opcode));
            //and reg16/immed16 has a fixed reg field
            machineCode.append(numToHexStr(modRegRmGenerator(0X3, 0X4, getGpRegCode(dest, destType))));
            machineCode.append(numToHexStr(src));
            return {machineCode, true, ""};
        }
    }

    else if(destType==OperandType::Mem8 || srcType==OperandType::Mem8 ||
            destType==OperandType::Mem16 || srcType==OperandType::Mem16) {

        bool destIsMemAddr = destType == OperandType::Mem8 || destType == OperandType::Mem16;

        QStringList addrArgs;
        uchar reg = 0X7;


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

        if(displacment.empty()) mod = 0x00;         //for bx+si, bp+di ... and si, di as well
        else if(!displacment.empty()) {
            if(displacment.size() == 1 && addrArgs.size() == 1) mod = 0x00; //direct address;
            else mod = (isMod1(displacmentValue) ? 0x01 : 0x02);       //the rest of the cases!
        }

        if(destIsMemAddr) {
            if(srcType == OperandType::SegReg)
                reg = getSegRegCode(src);
            else if(srcType == OperandType::Reg16 || srcType == OperandType::Reg8)
                reg = getGpRegCode(src, srcType);
            else if(srcType == OperandType::Indexer) {
                reg = indexersHex.find(src.toStdString())->second;
                generalExpression = "MEM16-REG16";
            }
            else if(srcType == Immed8 || srcType == Immed16)
                reg = 0x04;

            //this line is a motherfucker
            srcType == Immed8 ? reg = 0x4 : reg = reg; //And reg8/mem8, immed8 has a fixed reg
            uchar rm = rmGenerator(dest);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        } else {
            if(destType == OperandType::SegReg)
                reg = getSegRegCode(dest);
            else if(destType == OperandType::Reg16 || destType == OperandType::Reg8)
                reg = getGpRegCode(dest, destType);
            else if(destType == OperandType::Indexer) {
                reg = indexersHex.find(dest.toStdString())->second;
                generalExpression = "REG16-MEM16";
            }

            else
                reg = getGpRegCode(dest, destType);
            uchar rm = rmGenerator(src);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        }

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        machineCode.append(numToHexStr(modregrm));
        if(!displacment.empty())
            machineCode.append(numToHexStr(displacment.first(), ((directAddress || mod == 0x02) ? OutputSize::Word : OutputSize::Byte)));
        if(srcType == OperandType::Immed8)
            machineCode.append(numToHexStr(src, OutputSize::Byte));
        else if (srcType == OperandType::Immed16)
            machineCode.append(numToHexStr(src, OutputSize::Word));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg16 && srcType==OperandType::SegReg) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(src), getGpRegCode(dest, destType));
        machineCode.append(numToHexStr(opcode)); machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::SegReg && srcType==OperandType::Reg16) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(dest), getGpRegCode(src, srcType));
        machineCode.append(numToHexStr(opcode)); machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType == OperandType::Reg16 && srcType == OperandType::Indexer) {
        opcode = getOpcode("REG16-REG16", &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        auto indexerCode = indexersHex.find(src.toStdString())->second;
        modregrm = modRegRmGenerator(0x3, indexerCode, getGpRegCode(dest, OperandType::Reg16));
        machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType == OperandType::Indexer && srcType == OperandType::Reg16) {
        opcode = getOpcode("REG16-REG16", &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        auto indexerCode = indexersHex.find(dest.toStdString())->second;
        modregrm = modRegRmGenerator(0x3, getGpRegCode(src, OperandType::Reg16), indexerCode);
        machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }
    return {"", false, "Invalid Operands"};
}

uchar And::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toUpper().toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1;
}

And::And(const QString& param) {
    this->setCodeLine(param);
}
And::And() {tokens = 3;}
