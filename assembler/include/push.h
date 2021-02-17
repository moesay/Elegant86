#ifndef PUSH_H
#define PUSH_H

#include "base.h"

class Push : public Base {
public:
    Push(const QString&);
    Push();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;

private:
    std::unordered_map<std::string, uchar> LUT{
        {"ES", 0X06}, {"CS", 0X0E}, {"SS", 0X16}, {"MEM16", 0XFF},
            {"DS", 0X1E}, {"AX", 0X50}, {"CX", 0X51}, {"DX", 0X52},
            {"BX", 0X53}, {"SP", 0X54}, {"BP", 0X55}, {"SI", 0X56},
            {"DI", 0X57}
    };
};

#endif
