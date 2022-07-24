#ifndef MOV_H
#define MOV_H

#include "base.h"

class Mov : public Base {
public:
    Mov(const QString&);
    Mov();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8-r8",   0X88}, {"m8-r8",     0X88},
            {"r16-r16", 0X89}, {"m16-r16",    0X89},
            {"r8-r8",   0X8A}, {"r8-m8",     0X8A},
            {"r16-r16", 0X8B}, {"r16-m16",    0X8B},
            {"m8-i8",  0XC6}, {"m16-i16",  0XC7},

            {"r16-sr", 0X8C}, {"m16-sr",   0X8C},
            {"sr-m16",   0X8E}, {"sr-r16", 0X8E},

            {"AL-m8",         0XA0}, {"AX-m16", 0XA1},
            {"m8-AL",         0XA2}, {"m16-AX", 0XA3},

            {"AL-i8",   0XB0}, {"CL-i8",     0XB1},
            {"DL-i8",   0XB2}, {"BL-i8",     0XB3},
            {"AH-i8",   0XB4}, {"CH-i8",     0XB5},
            {"DH-i8",   0XB6}, {"BH-i8",     0XB7},
            {"AX-i16",  0XB8}, {"CX-i16",    0XB9},
            {"DX-i16",  0XBA}, {"BX-i16",    0XBB},
            {"SP-i16",  0XBC}, {"BP-i16",    0XBD},
            {"SI-i16",  0XBE}, {"DI-i16",    0XBF}};
};

#endif
