#include <assembler/include/base.h>
#include <assembler/include/no-op-inst.h>

InstRet_T No_OP_Inst::process() {
    bool state = false;
    opcode = getOpcode(codeLine.trimmed(), &state);
    if(state == true)
        return {numToHexStr(opcode), true, ""};

    return {"", false, "Unknown Instruction"};
}

uchar No_OP_Inst::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toUpper().toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; //not used
}

No_OP_Inst::No_OP_Inst() {tokens = 0;}
No_OP_Inst::No_OP_Inst(const QString& param) {
    tokens = 0;
    this->setCodeLine(param);
}
