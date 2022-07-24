#ifndef POP_H
#define POP_H

#include "base.h"

class Pop : public Base {
public:
    Pop(const QString&);
    Pop();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;

private:
    std::unordered_map<std::string, uchar> LUT{
        {"ES", 0X07}, {"SS", 0X17}, {"DS", 0X1F}, {"AX", 0X58},
            {"CX", 0X59}, {"DX", 0X5A}, {"BX", 0X5B}, {"SP", 0X5C},
            {"BP", 0X5D}, {"SI", 0X5E}, {"DI", 0X5F}, {"r16", 0X8F},
            {"m16", 0X8F}
    };
};

#endif
