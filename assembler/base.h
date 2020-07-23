#ifndef BASE_H
#define BASE_H


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

template <typename T>
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
    MemAddr,
    Reg8, Reg16,
    Immed8, Immed16,
    SegReg, NegImmed8,
    NegImmed16, Label,
    NOP, Char, Unknown
};

using label = std::tuple<int, QString, QString>;
extern QList<label> lbls;

const static std::array<QString, 12> Operands{
        "MEM", "REG8", "REG16", "IMMED8", "IMMED16", "SEGREG",
        "NEGIMMED8", "NEGIMMED16", "LABEL", "NOP", "CHAR", "UNKNOWN"};

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

class Base {
    protected:
        Base(const QString& codeLine) : codeLine(codeLine){}
        Base();
    public:
        virtual QString process() = 0;
        bool isImmed8(const QString&);
        bool isChar(const QString&);
        void hexValidator(QStringList&);
        bool isImmed16(const QString&);
        Base& get();
        bool isHexValue(const QString&);
        QString extractDisplacment(const QString&, bool *ok = nullptr);
        bool isMemAddr(const QString&);
        enum OperandType getOperandType(const QString&);
        std::tuple<QString, QString> twoTokens();
        std::tuple<QString, QString, QString> threeTokens();
        const QString& getCodeLine();
        uchar getSegRegCode(const QString&, bool *ok = nullptr);
        void setCodeLine(const QString&);
        QString signHandler(QString&&, const OperandType&);
        bool registerMod(const QString&);
        uchar modRegRmGenerator(const uchar&, const uchar&, const uchar&);
        uchar rmGenerator(const QString&);
        virtual uchar getOpcode(const QString&, bool *ok = nullptr)=0;
        uchar getGpRegCode(const QString&, bool *ok = nullptr);
        virtual ~Base() = 0;
    private:
        int x;
    protected:
        QString codeLine;
};

#endif
