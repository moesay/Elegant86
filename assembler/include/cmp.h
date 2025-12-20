#ifndef CMP_H
#define CMP_H

#include "base.h"

class Cmp : public Base {
public:
    Cmp(const QString&);
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    Cmp();

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8-r8",       0X38},      {"m8-r8",       0X38},
            {"r16-r16",     0X39},      {"m16-r16",     0X39},
            {"r8-m8",       0X3A},      {"r16-m16",     0X3B},
            {"AL-i8",       0X3C},      {"AX-i16",      0X3D},
            {"r8-i8",       0X80},      {"m8-i8",       0X80},
            {"r16-i16",     0X81},      {"m16-i16",     0X81},
            {"r16-i8",      0X83},      {"m16-i8",      0X83}};
};

#endif
