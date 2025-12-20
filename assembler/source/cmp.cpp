#include <assembler/include/base.h>
#include <assembler/include/cmp.h>

InstRet_T Cmp::process() {
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

    // Get the operand types
    destType = getOperandType(dest); srcType = getOperandType(src);

    // Use refactored helper to resolve memory operand sizes
    if(!resolveMemoryOperandSize(destType, srcType))
        return {"", false, "Unknown pointer size"};

    // Build general expression
    generalExpression = Operands[destType] + '-' + Operands[srcType];

    // Register to register - use refactored helper
    if(((destType==OperandType::Reg16) && (srcType==OperandType::Reg16)) ||
            ((destType==OperandType::Reg8) && (srcType==OperandType::Reg8))) {
        return processRegToReg();
    }

    // Register with immediate (8-bit) - use refactored helper
    else if(destType==OperandType::Reg8 && srcType==OperandType::Immed8) {
        return processRegWithImmed8(dest, src, 0x07); // CMP uses reg field = 7
    }

    // Register with immediate (16-bit) - use refactored helper
    else if(destType==OperandType::Reg16 && (srcType==OperandType::Immed16 || srcType==Immed8)){
        return processRegWithImmed16(dest, src, 0x07); // CMP uses reg field = 7
    }

    // Memory operands - use refactored helper
    else if(destType==OperandType::Mem8 || srcType==OperandType::Mem8 ||
            destType==OperandType::Mem16 || srcType==OperandType::Mem16) {
        return processMemoryOperand(dest, src, 0x07); // CMP uses reg field = 7
    }

    return {"", false, "Invalid Operands"};
}

uchar Cmp::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; // not used
}

Cmp::Cmp(const QString& param) {
    this->setCodeLine(param);
    tokens = 3;
}
Cmp::Cmp() {tokens = 3;}
