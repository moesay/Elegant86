#include <assembler/include/base.h>
#include <assembler/include/push.h>

InstRet_T Push::process() {

    bool state = true;
    uchar modregrm;
    uchar mod = 0x00;
    QString machineCode;
    uchar opcode;

    try {
        twoTokens();
    }
    catch(InvalidPointer &ex) {
        return {"", false, ex.what()};
    }

    auto [mnemonic, dest] = twoTokens();

    try {
        segmentPrefixWrapper(dest, machineCode);
    } catch (InvalidSegmentOverridePrefix& exc) {
        return {"", false, exc.what()};
    }

    //get the operand type. is it a reg16, reg8, mem address, segreg or what?
    OperandType destType = getOperandType(dest);

    if(pointerType == Pointer::Word && destType != OperandType::Reg16)
        destType = OperandType::Mem16;

    QString generalExpression = Operands[destType];

    if(destType == OperandType::Reg16  ||
            destType == OperandType::SegReg ||
            destType == OperandType::Indexer) {

        opcode = getOpcode(dest, &state);
        if(state == false) return {"", false, "Invalid Operands"};

        machineCode.append(hexToStr(opcode));
        return {machineCode, true, ""};
    }

    else if(destType == OperandType::Mem16) {
        if(pointerType != Pointer::Word)
            return {"", false, "Invalid Operands"};
        QStringList addrArgs;
        uchar reg;

        dest.remove("["); dest.remove("]");
        addrArgs = dest.split(QRegExp("[+]"));

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

        reg = 0X06;
        uchar rm = rmGenerator(dest);
        if(rm == 0xFF) return {"", false, "Unhandeled Error"};
        modregrm = modRegRmGenerator(mod, reg, rm);

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};

        machineCode.append(hexToStr(opcode));
        machineCode.append(hexToStr(modregrm));
        if(!displacment.empty())
            machineCode.append(hexToStr(displacment.first().toInt(0, 16), ((directAddress || mod == 0x02) ? OutputSize::Word : OutputSize::Byte)));
        return {machineCode, true, ""};
    }

    return {"", false, "Invalid Operands"};
}

uchar Push::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toUpper().toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; //not used
}

Push::Push(const QString& param) {
    this->setCodeLine(param);
}

Push::Push() {}
