#ifndef MOV_H
#define MOV_H

#include "base.h"

class Mov : public Base {
    public:
        Mov();
        /* using Base::Base; */
        QString process() override;
        uchar getOpcode(const QString&, bool* ok = nullptr) override;
        ~Mov() override;

    private:
        std::unordered_map<std::string, uchar> LUT{
            //all the reg-reg are done
                {"REG8-REG8",   0X88}, {"MEM-REG8",     0X88},
                {"REG16-REG16", 0X89}, {"MEM-REG16",    0X89},
                {"REG8-REG8",   0X8A}, {"REG8-MEM",     0X8A},
                {"REG16-REG16", 0X8B}, {"REG16-MEM",    0X8B},
                {"MEM-IMMED8",  0XC6}, {"MEM-IMMED16",  0XC7},

                {"REG16-SEGREG", 0X8C}, {"MEM-SEGREG",   0X8C},
                {"SEGREG-MEM",   0X8E}, {"SEGREG-REG16", 0X8E},

                {"AL-MEM",         0XA0}, {"AX-MEM", 0XA1},
                {"MEM-AL",         0XA2}, {"MEM-AX", 0XA3},  //mem8 mem16

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
