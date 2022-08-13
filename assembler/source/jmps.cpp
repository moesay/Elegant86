#include <assembler/include/base.h>
#include <assembler/include/jmps.h>

InstRet_T Jmps::process() {
    machineCode.clear();

    std::optional<std::tuple<QString, QString, QString>> temp = tokenize();

    if(temp == std::nullopt)
        return {"", false, "Invalid Pointer"};

    auto [mnemonic, label, t] = *temp;

    uint16_t ip = info.relAddr;
    destType = getOperandType(label);
    if((destType != OperandType::Label) && (destType != OperandType::Immed8))
        return {"", false, "Invalid Operands"};

    if(destType == OperandType::Label) {
        uint16_t labelAddr = Labels::getLabelAddr(label);

        if(labelAddr > ip) {
            if((labelAddr - ip) >= 128) {
                machineCode.append(numToHexStr(getOpcode(negate(mnemonic), &state)));
                machineCode.append(numToHexStr(3, OutputSize::Byte));
                machineCode.append("E9");
                machineCode.append(numToHexStr(static_cast<int>(labelAddr) - ip, OutputSize::Word));
            }
            else {
                machineCode.append(numToHexStr(opcode));
                machineCode.append(numToHexStr(static_cast<int>(labelAddr) - ip, OutputSize::Byte));
            }
        }
        else if(labelAddr < ip) {
            if((ip - labelAddr) >= 129) {
                return{machineCode, false, "Backward long/far jums aren't supported yet."};
            } else {
                ip = ~(ip - labelAddr) + 1;
                machineCode.append(numToHexStr(getOpcode(mnemonic, &state)));
                machineCode.append(numToHexStr(static_cast<int>(ip), OutputSize::Byte));
            }
        }
        return {machineCode, true, ""};
    }
    else if(destType == OperandType::Immed8) {
        machineCode.append(numToHexStr(mnemonic));
        int disp = label.toInt(0, 16) - 2;
        machineCode.append(numToHexStr(disp));

        return {machineCode, true, ""};
    }

    generalExpression = Operands[destType];

    return {"", false, "Invalid Operands"};
}

uchar Jmps::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; //not used
}

QString Jmps::negate(const QString& inst) {
    for(int i = 0; i < neg.size(); i++) {
        if(neg[i] == inst.toStdString()) {
            if (i%2 == 0)
                return QString::fromStdString(neg[i+1]);
            else
                return QString::fromStdString(neg[i-1]);
        }
    }
    return "";
}

Jmps::Jmps(const QString& param) {
    this->setCodeLine(param);
    tokens = 2;
}

Jmps::Jmps() {tokens = 2;}
