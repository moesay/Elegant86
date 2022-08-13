#ifndef JMP_H
#define JMP_H

#include "base.h"

class Jmp : public Base {
public:
    Jmp(const QString&);
    Jmp();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;

private:
    const std::unordered_map<std::string, uchar> LUT{
        {"SHORT", 0XEB}, {"LONG", 0XE9}, {"FAR", 0XEA}
    };
};

#endif
