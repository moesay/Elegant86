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
        //all the reg-reg are done
        {"REG8-REG8",   0X88}, {"MEM8-REG8",     0X88},
            {"REG16-REG16", 0X89}, {"MEM16-REG16",    0X89},
            {"REG8-REG8",   0X8A}, {"REG8-MEM8",     0X8A},
            {"REG16-REG16", 0X8B}, {"REG16-MEM16",    0X8B},
            {"MEM8-IMMED8",  0XC6}, {"MEM16-IMMED16",  0XC7},

            {"REG16-SEGREG", 0X8C}, {"MEM16-SEGREG",   0X8C},
            {"SEGREG-MEM16",   0X8E}, {"SEGREG-REG16", 0X8E},

            {"AL-MEM8",         0XA0}, {"AX-MEM16", 0XA1},
            {"MEM8-AL",         0XA2}, {"MEM16-AX", 0XA3},  //mem8 mem16

            //done
            {"AL-IMMED8",   0XB0}, {"CL-IMMED8",     0XB1},
            {"DL-IMMED8",   0XB2}, {"BL-IMMED8",     0XB3},
            {"AH-IMMED8",   0XB4}, {"CH-IMMED8",     0XB5},
            {"DH-IMMED8",   0XB6}, {"BH-IMMED8",     0XB7},
            {"AX-IMMED16",  0XB8}, {"CX-IMMED16",    0XB9},
            {"DX-IMMED16",  0XBA}, {"BX-IMMED16",    0XBB},
            {"SP-IMMED16",  0XBC}, {"BP-IMMED16",    0XBD},
            {"SI-IMMED16",  0XBE}, {"DI-IMMED16",    0XBF}};
};

#endif
