#ifndef ADD_H
#define ADD_H

#include "base.h"

class Add : public Base {
public:
    Add(const QString&);
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    Add();

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8-r8",       0X00},      {"m8-r8",       0X00},
            {"r16-r16",     0X01},      {"m16-r16",     0X01},
            {"r8-r8",       0X02},      {"r8-m8",       0X02},
            {"r16-r16",     0X03},      {"r16-m16",     0X03},
            {"AL-i8",       0X04},      {"AX-i16",      0X05},
            {"r8-i8",     0X80},      {"m8-i8",     0X80},
            {"r16-i16",   0X81},      {"m16-i16",   0X81},
            {"r8-i8",     0X82},      {"m8-i8",     0X82},
            {"r16-i16",   0X83},      {"m16-i16",   0X83},};
};

#endif
