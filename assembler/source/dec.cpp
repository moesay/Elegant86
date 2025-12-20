#include <assembler/include/base.h>
#include <assembler/include/dec.h>

InstRet_T Dec::process() {
    machineCode.clear();

    std::optional<std::tuple<QString, QString, QString>> temp = tokenize();

    if(temp == std::nullopt)
        return {"", false, "Invalid Pointer"};
    auto [mnemonic, dest, src] = *temp;

    // DEC only has one operand
    if(!src.isEmpty())
        return {"", false, "DEC takes only one operand"};

    try {
        segmentPrefixWrapper(dest, src);
    } catch(InvalidSegmentOverridePrefix& exc) {
        return {"", false, exc.what()};
    }

    // Get the operand type
    destType = getOperandType(dest);

    // Handle memory operands with pointer type
    if(destType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? destType = OperandType::Mem8 : destType = OperandType::Mem16;
        else
            return {"", false, "Unknown pointer size"};
    }

    // Special case: 16-bit registers have short encoding (1 byte)
    if(destType == OperandType::Reg16) {
        bool state = false;
        opcode = getOpcode(dest, &state);
        if(state == true) {
            machineCode.append(numToHexStr(opcode));
            return {machineCode, true, ""};
        }
        // Fall through to general form if special encoding not found
    }

    // General form for r/m8 and r/m16
    if(destType == OperandType::Reg8 || destType == OperandType::Reg16) {
        generalExpression = Operands[destType];
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};

        reg = 0x01; // DEC uses reg field = 1
        modregrm = modRegRmGenerator(0x3, reg, getGpRegCode(dest, destType));
        machineCode.append(numToHexStr(opcode));
        machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    // Memory operands
    else if(destType == OperandType::Mem8 || destType == OperandType::Mem16) {
        dest.remove("["); dest.remove("]");
        QStringList addrArgs = dest.split(QRegExp("[+]"));

        QStringList displacment = addrArgs.filter(QRegularExpression("[0-9a-fA-F]"));
        if(std::any_of(std::begin(addrArgs), std::end(addrArgs), [](const QString& p) {
                    return (notAdressingRegs.contains(p) || Regs8.contains(p));
                    })) return {"", false, "Invalid addressing register/s"};
        hexValidator(displacment);

        bool directAddress = addrArgs.size() == 1;
        int displacmentValue = 0;

        if(displacment.size() >= 1)
            displacmentValue = displacment.first().toInt(0, 16);

        if(displacment.empty()) mod = 0x00;
        else if(!displacment.empty()) {
            if(displacment.size() == 1 && addrArgs.size() == 1) mod = 0x00; // direct address
            else mod = (isMod1(displacmentValue) ? 0x01 : 0x02);
        }

        generalExpression = Operands[destType];
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};

        reg = 0x01; // DEC uses reg field = 1
        uchar rm = rmGenerator(dest);
        if(rm == 0xFF) return {"", false, "Unhandeled Error"};
        modregrm = modRegRmGenerator(mod, reg, rm);

        machineCode.append(numToHexStr(opcode));
        machineCode.append(numToHexStr(modregrm));
        if(!displacment.empty())
            machineCode.append(numToHexStr(displacment.first(), ((directAddress || mod == 0x02) ? OutputSize::Word : OutputSize::Byte)));

        return {machineCode, true, ""};
    }

    return {"", false, "Invalid Operands"};
}

uchar Dec::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; // not used
}

Dec::Dec(const QString& param) {
    this->setCodeLine(param);
    tokens = 2;
}
Dec::Dec() {tokens = 2;}
