#ifndef PARSER_H
#define PARSER_H


#include <QRegularExpression>
#include <QByteArray>
#include <QDebug>
#include <tuple>
#include <QString>
#include <unordered_map>
#include <sstream>

enum HexType {
    Address, OpCode, DirectAddress
};

template <class T>
QString hexToStr (T param, HexType ht = HexType::OpCode) {
    std::stringstream ss;
    //added for aligment

    /* if(param >= 0xFF80) { */
    /*     param &= (param-0xff00); */
    /* } */

    if(param > 0xFF) {
        ss << std::hex << ((param & 0x00f0) >> 4);
        ss << std::hex << ((param & 0x000f) >> 0);
        ss << std::hex << ((param & 0xf000) >> 12);
        ss << std::hex << ((param & 0x0f00) >> 8);
    } else {
        ss << std::hex << ((param & 0x00f0) >> 4);
        ss << std::hex << ((param & 0x000f) >> 0);
    }

    if ((param > 0x7F) && (param < 0xFF80) && (ht==HexType::Address && ss.str().length() != 4) && (param != 0xFF)) {
        ss.seekg(0);
        ss << "00";
    }

    //the direct address has to be two bytes.
    if(ht == HexType::DirectAddress) {
        ss.seekg(0);
        while(ss.str().length() < 4) ss << "0";
    }

    if (ss.str().length() % 2) {
        return "0" + QString::fromStdString(ss.str()).toUpper();
    }
    return QString::fromStdString(ss.str()).toUpper();
}

enum OperandType {
    Mem8, Mem16,
    Reg8, Reg16,
    Immed8, Immed16,
    segReg, Unknown
};

const static std::array<QString, 9> Operands{"MEM", "MEM", "REG8", "REG16", "IMMED8", "IMMED16", "SEGREG", "UNKNOWN"};

const std::unordered_map<std::string, uchar>segRegsHex { {"ES", 0x00}, {"CS", 0x01}, {"SS", 0x02}, {"DS", 0x03} };

static std::unordered_map<std::string, uchar> mod00 {
        {"BX+SI", 0x00}, {"BX+DI", 0x01}, {"BP+SI", 0x02}, {"BP+DI", 0x03},
        {"SI",    0x04}, {"DI",    0x05}, {"DA",    0x06}, {"BX",    0x07}};

static std::unordered_map<std::string, uchar> gpRegsHex {
        {"AL", 0x00}, {"AX", 0x00}, {"CL", 0x01}, {"CX", 0x01},
        {"DL", 0x02}, {"DX", 0x02}, {"BL", 0x03}, {"BX", 0x03},
        {"AH", 0x04}, {"SP", 0x04}, {"CH", 0x05}, {"BP", 0x05},
        {"DH", 0x06}, {"SI", 0x06}, {"BH", 0x07}, {"DI", 0x07}};

const static QVector<QString> Regs16 {"AX", "BX", "CX", "DX", "SP", "BP", "SI", "DI"};
const static QVector<QString> Regs8  {"AH", "AL", "BH", "BL", "CH", "CL", "DH", "DL"};
const static QVector<QString> SegRegs {"ES", "CS", "SS", "DS"};

class Instruction {
    public:
        Instruction(const QString& codeLine) : codeLine(codeLine){}
        Instruction();
        virtual QString process() = 0;
        bool isImmed8(const QString&);
        bool isImmed16(const QString&);
        QString extractDisplacment(const QString&, bool *ok = nullptr);
        bool isMemAddr(const QString&);
        enum OperandType getOperandType(const QString&);
        std::tuple<QString, QString> twoTokens();
        std::tuple<QString, QString, QString> threeTokens();
        const QString& getCodeLine();
        uchar getSegRegCode(const QString&, bool *ok = nullptr);
        void setCodeLine(const QString&);
        bool registerMod(const QString&);
        uchar modRegRmGenerator(const uchar&, const uchar&, const uchar&);
        uchar rmGenerator(const QString&);
        virtual uchar getOpcode(const QString&, bool *ok = nullptr)=0;
        uchar getGpRegCode(const QString&, bool *ok = nullptr);
        virtual ~Instruction();
    private:

    protected:
        QString codeLine;
};


class Mov : public Instruction {
    public:
        Mov();
        using Instruction::Instruction;
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

                {"AL-MEM",         0XA0}, {"AX-MEM",          0XA1},
                {"MEM-AL"/*MEM8*/, 0XA2}, {"MEM-AX"/*MEM16*/, 0XA3},

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
