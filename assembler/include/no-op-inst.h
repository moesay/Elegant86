#ifndef NO_OP_INST_H
#define NO_OP_INST_H

#include "base.h"

class No_OP_Inst : public Base {
public:
    No_OP_Inst();
    No_OP_Inst(const QString&);
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    InstRet_T process() override;

private:
    std::unordered_map<std::string, uchar> LUT{
        {"AAA", 0X37}, {"AAD", 0XD5}, {"AAM", 0XD4}, {"CBW", 0X98},
        {"CLC", 0XF8}, {"CLD", 0XFC}, {"CLI", 0XFA}, {"CMC", 0XF5},
        {"CWD", 0X99}, {"DAA", 0X27}, {"DAS", 0X2F}, {"HLT", 0XF4},
        {"INTO", 0XCE}, {"IRET", 0XCF}, {"LAHF", 0XF9}, {"LOCK", 0XF0},
        {"NOP", 0X90}, {"POPF", 0X9D}, {"PUSHF", 0X9C}, {"SAHF", 0X9E},
        {"STC", 0XF9}, {"STD", 0XFD}, {"STI", 0XFB}, {"WAIT", 0X9B},
        {"AAS", 0X3F}
    };
};

#endif
