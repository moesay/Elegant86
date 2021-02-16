#ifndef ADD_H
#define ADD_H

#include "base.h"

class Add : public Base {
    public:
        Add(const QString&);
        InstRet process() override;
        uchar getOpcode(const QString&, bool* ok = nullptr) override;
        Add();

    private:
        std::unordered_map<std::string, uchar> LUT{
                {"REG8-REG8",       0X00},      {"MEM8-REG8",       0X00},
                {"REG16-REG16",     0X01},      {"MEM16-REG16",     0X01},
                {"REG8-REG8",       0X02},      {"REG8-MEM8",       0X02},
                {"REG16-REG16",     0X03},      {"REG16-MEM16",     0X03},
                {"AL-IMMED8",       0X04},      {"AX-IMMED16",      0X05}, //--
                {"REG8-IMMED8",     0X80},      {"MEM8-IMMED8",     0X80},
                {"REG16-IMMED16",   0X81},      {"MEM16-IMMED16",   0X81},
                {"REG8-IMMED8",     0X82},      {"MEM8-IMMED8",     0X82},
                {"REG16-IMMED16",   0X83},      {"MEM16-IMMED16",   0X83},};
};

#endif
