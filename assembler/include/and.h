#ifndef AND_H
#define AND_H

#include "base.h"

class And : public Base {
public:
    And(const QString&);
    And();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;

private:
    std::unordered_map<std::string, uchar> LUT{
        {"REG8-REG8", 0X20}, {"MEM8-REG8", 0X20}, {"REG16-REG16", 0X21}, {"MEM16-REG16", 0X21},
        {"REG8-REG8", 0X22}, {"REG8-MEM8", 0X22}, {"REG16-REG16", 0X23}, {"REG16-MEM16", 0X23},
        {"AL-IMMED8", 0X24}, {"AX-IMMED16", 0X25}, {"REG8-IMMED8", 0X80}, {"MEM8-IMMED8", 0X80},
        {"REG16-IMMED16", 0X81}, {"MEM16-IMMED16", 0X81},

    };
};

#endif
