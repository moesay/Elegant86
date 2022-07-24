#ifndef SUB_H
#define SUB_H

#include "base.h"

class Sub : public Base {
public:
    Sub(const QString&);
    Sub();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;

private:
    std::unordered_map<std::string, uchar> LUT{
        {"r8-r8", 0X28}, {"m8-r8", 0X28}, {"r16-r16", 0X29},
        {"m16-r16", 0X29}, {"r8-r8", 0X2A}, {"r8-m8", 0X2A},
        {"r16-r16", 0X2B}, {"r16-m16", 0X2B}, {"AL-i8", 0X2C},
        {"AX-i16", 0X2D}, {"r8-i8", 0X80}, {"m8-i8", 0X80},
        {"r16-i16", 0X81}, {"m16-i16", 0X81}, {"r8-i8", 0X82},
        {"m8-i8", 0X82}, {"r16-i8", 0X83}, {"m16-i8", 0X83}};
};

#endif
