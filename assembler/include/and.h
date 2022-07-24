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
        {"r8-r8", 0X20}, {"m8-r8", 0X20}, {"r16-r16", 0X21}, {"m16-r16", 0X21},
        {"r8-r8", 0X22}, {"r8-m8", 0X22}, {"r16-r16", 0X23}, {"r16-m16", 0X23},
        {"AL-i8", 0X24}, {"AX-i16", 0X25}, {"r8-i8", 0X80}, {"m8-i8", 0X80},
        {"r16-i16", 0X81}, {"m16-i16", 0X81},

    };
};

#endif
