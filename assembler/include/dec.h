#ifndef DEC_H
#define DEC_H

#include "base.h"

class Dec : public Base {
public:
    Dec(const QString&);
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    Dec();

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8",   0XFE},      {"m8",   0XFE},
            {"r16",  0XFF},      {"m16",  0XFF},
            // Special short encoding for 16-bit registers
            {"AX",   0X48},      {"CX",   0X49},
            {"DX",   0X4A},      {"BX",   0X4B},
            {"SP",   0X4C},      {"BP",   0X4D},
            {"SI",   0X4E},      {"DI",   0X4F}};
};

#endif
