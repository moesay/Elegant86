#ifndef OR_H
#define OR_H

#include "base.h"

class Or : public Base {
public:
    Or(const QString&);
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    Or();

private:
    std::unordered_map<std::string, uchar> LUT{
            {"r8-r8",       0X08},      {"m8-r8",       0X08},
            {"r16-r16",     0X09},      {"m16-r16",     0X09},
            {"r8-m8",       0X0A},      {"r16-m16",     0X0B},
            {"AL-i8",       0X0C},      {"AX-i16",      0X0D},
            {"r8-i8",       0X80},      {"m8-i8",       0X80},
            {"r16-i16",     0X81},      {"m16-i16",     0X81}};
};

#endif
