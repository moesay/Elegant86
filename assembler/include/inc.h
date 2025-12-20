#ifndef INC_H
#define INC_H

#include "base.h"

class Inc : public Base {
public:
    Inc(const QString&);
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    Inc();

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8",   0XFE},      {"m8",   0XFE},
            {"r16",  0XFF},      {"m16",  0XFF},
            // Special short encoding for 16-bit registers
            {"AX",   0X40},      {"CX",   0X41},
            {"DX",   0X42},      {"BX",   0X43},
            {"SP",   0X44},      {"BP",   0X45},
            {"SI",   0X46},      {"DI",   0X47}};
};

#endif
