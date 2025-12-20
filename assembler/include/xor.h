#ifndef XOR_H
#define XOR_H

#include "base.h"

class Xor : public Base {
public:
    Xor(const QString&);
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    Xor();

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8-r8",       0X30},      {"m8-r8",       0X30},
            {"r16-r16",     0X31},      {"m16-r16",     0X31},
            {"r8-m8",       0X32},      {"r16-m16",     0X33},
            {"AL-i8",       0X34},      {"AX-i16",      0X35},
            {"r8-i8",       0X80},      {"m8-i8",       0X80},
            {"r16-i16",     0X81},      {"m16-i16",     0X81}};
};

#endif
