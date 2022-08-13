#include <assembler/include/base.h>
#include <assembler/include/jmp.h>

InstRet_T Jmp::process() {
    machineCode.clear();
    bool seg;

    std::optional<std::tuple<QString, QString, QString>> temp = tokenize();

    if(temp == std::nullopt)
        return {"", false, "Invalid Pointer"};

    auto [mnemonic, op1, op2] = *temp;

    try {
        seg = segmentPrefixWrapper(op2, op1);
    } catch(InvalidSegmentOverridePrefix& exc) {
        return {"", false, exc.what()};
    }

    uint16_t ip = info.relAddr;

    srcType = getOperandType(op1); destType = getOperandType(op2);

    if(srcType == OperandType::Label ||
            (srcType == OperandType::JShort && destType == OperandType::Label)) {
        uint16_t labelAddr = srcType == OperandType::Label ? Labels::getLabelAddr(op1) : Labels::getLabelAddr(op2);
        if(labelAddr > ip) {
            if((labelAddr - ip) >= 128) {
                return {"", false, "A short jump is used with a long/far label."};
            }
            else {
                machineCode.append(numToHexStr(getOpcode("SHORT", &state)));
                machineCode.append(numToHexStr(static_cast<int>(labelAddr) - ip, OutputSize::Byte));
                return {machineCode, true, ""};
            }
        }
        else if(ip > labelAddr) {
            if(seg) ip++;
            if((ip - labelAddr) > 128) {
                return {"", false, "A short jump is used with a long/far label."};
            }
            else {
                ip = ~(ip - labelAddr) + 1;
                machineCode.append(numToHexStr(getOpcode("SHORT", &state)));
                machineCode.append(numToHexStr(static_cast<int>(ip), OutputSize::Byte));
                return {machineCode, true, ""};
            }
        }
    }
    else if(srcType == OperandType::Immed8) {
        machineCode.append(numToHexStr(getOpcode("SHORT", &state)));
        int disp = op1.toInt(nullptr, 16) - ip;
        machineCode.append(numToHexStr(disp, OutputSize::Byte));
        return {machineCode, true, ""};

    }
    else if(srcType == OperandType::JLong) {
        if(destType == OperandType::Label) {
            uint16_t labelAddr = Labels::getLabelAddr(op2);
            if(labelAddr > ip) {
                machineCode.append(numToHexStr(getOpcode("LONG", &state)));
                machineCode.append(numToHexStr(static_cast<int>(labelAddr) - ip, OutputSize::Word));
                return {machineCode, true, ""};
            }
            else {
                ip = ~(ip - labelAddr) + 1;
                machineCode.append(numToHexStr(getOpcode("LONG", &state)));
                machineCode.append(numToHexStr(static_cast<int>(ip), OutputSize::Word));
                return {machineCode, true, ""};
            }
        }
        else if(destType == OperandType::Immed16) {
            machineCode.append(numToHexStr(getOpcode("LONG", &state)));
            int disp = op2.toInt(nullptr, 16) - ip;
            machineCode.append(numToHexStr(disp, OutputSize::Word));
            return {machineCode, true, ""};
        }
    }
    else if(srcType == OperandType::JFar) {
        if(isSegOffset(op2)){
            QStringList temp = op2.split(':');
            machineCode.append(numToHexStr(getOpcode("FAR", &state)));
            machineCode.append((numToHexStr(temp.at(1), OutputSize::Word)));
            machineCode.append((numToHexStr(temp.at(0), OutputSize::Word)));

            return {machineCode, true, ""};
        }
        return {"", false, "Invalid operands for a far jump."};
    }

    return {"", false, "Invalid Operands"};
}

uchar Jmp::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; //not used
}

Jmp::Jmp(const QString& param) {
    this->setCodeLine(param);
    tokens = 3;
}

Jmp::Jmp() {tokens = 3;}
