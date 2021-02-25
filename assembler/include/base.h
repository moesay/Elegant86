#ifndef BASE_H
#define BASE_H

#include <QRegularExpression>
#include <QByteArray>
#include <QDebug>
#include <tuple>
#include <concepts>
#include <QString>
#include <optional>
#include <unordered_map>
#include <sstream>
#include <include/exc.h>
#include <include/labels.h>

enum class Pointer : uint8_t {
    Byte, Word, None
};

enum class HexType : uint8_t {
    Address, OpCode, DirectAddress
};

enum class Sign : uint8_t {
    Pos, Neg
};

enum class OutputSize : uint8_t {
    Byte, Word, Dynamic
};

QString hexToStr (const std::integral auto &param, OutputSize opSize = OutputSize::Dynamic, Sign sign = Sign::Pos)
{
    std::stringstream ss;

    if(sign == Sign::Pos) {
        if(param > 0xFF) {
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
            ss << std::hex << ((param & 0xf000) >> 12);
            ss << std::hex << ((param & 0x0f00) >> 8);
        } else {
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
        }
    }
    else {
        if(param > 0xFF) {
            ss << std::hex << ((param & 0xf000) >> 12);
            ss << std::hex << ((param & 0x0f00) >> 8);
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
        } else {
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
        }
    }

    if(opSize != OutputSize::Dynamic) {
        if(opSize == OutputSize::Byte) {
            if(sign == Sign::Pos)
                while(ss.str().length() < 2) ss << "0";
            else if(sign == Sign::Neg)
                while(ss.str().length() < 2) ss << "F";
        }
        else if(opSize == OutputSize::Word) {
            if(sign == Sign::Pos)
                while(ss.str().length() < 4) ss << "0";
            else if(sign == Sign::Neg)
                while(ss.str().length() < 4) ss << "F";
        }
    }

    if (ss.str().length() % 2) {
        if(sign == Sign::Pos)
            return "0" + QString::fromStdString(ss.str()).toUpper();
        else
            return "F" + QString::fromStdString(ss.str()).toUpper();
    }
    return QString::fromStdString(ss.str()).toUpper();
}

enum OperandType : uint8_t {
    MemAddr, Mem8, Mem16,
    Reg8, Reg16,
    Immed8, Immed16,
    SegReg, NegImmed8,
    NegImmed16, Label,
    NOP, Char, Indexer, Invalid
};

using InstRet_T = std::tuple<QString, bool, QString>;
using Error_T = std::tuple<QString, QString, int>; //{Error Message, The word caused the error, the line number}

const static std::array<QString, 15> Operands{
        "MEM", "MEM8", "MEM16", "REG8", "REG16", "IMMED8", "IMMED16", "SEGREG",
        "NEGIMMED8", "NEGIMMED16", "LABEL", "NOP", "CHAR",  "INDEXER", "UNKNOWN"};

const  std::unordered_map<std::string, uchar> segRegsHex    { {"ES", 0x00}, {"CS", 0x01}, {"SS", 0x02}, {"DS", 0x03} };
const  std::unordered_map<std::string, uchar> indexersHex   { {"SP", 0x04}, {"BP", 0x05}, {"SI", 0x06}, {"DI", 0x07} };
const  std::unordered_map<std::string, uchar> segRegsPrefix { {"ES", 0X26}, {"CS", 0X2E}, {"SS", 0X36}, {"DS", 0X3E} };
static std::unordered_map<std::string, uchar> mod00 {
        {"BX+SI", 0x00}, {"BX+DI", 0x01}, {"BP+SI", 0x02}, {"BP+DI", 0x03},
        {"SI",    0x04}, {"DI",    0x05}, {"DA",    0x06}, {"BX",    0x07}};

static std::unordered_map<std::string, uchar> Regs16Hex {
        {"AX", 0x00}, {"CX", 0x01},
        {"DX", 0x02}, {"BX", 0x03},
        {"SP", 0x04}, {"BP", 0x05},
        {"SI", 0x06}, {"DI", 0x07}};

static std::unordered_map<std::string, uchar> Regs8Hex {
        {"AL", 0x00}, {"CL", 0x01},
        {"DL", 0x02}, {"BL", 0x03},
        {"AH", 0x04}, {"CH", 0x05},
        {"DH", 0x06}, {"BH", 0x07}};

const static QVector<QString> Regs16            {"AX", "BX", "CX", "DX", "SP", "BP", "SI", "DI"};
const static QVector<QString> Regs8             {"AH", "AL", "BH", "BL", "CH", "CL", "DH", "DL"};
const static QVector<QString> SegRegs           {"ES", "CS", "SS", "DS"};
const static QVector<QString> Indexers          {"SP", "BP", "SI", "DI"};
const static QVector<QString> notAdressingRegs  {"AX", "CX", "DX", "SP"};
const static QVector<QString> addressingRegs    {"BX", "SI", "DI", "BP"};

class Base {
    protected:
        explicit Base(const QString& codeLine) : codeLine(codeLine){}
        Base() = default;
        QString codeLine;
        Pointer pointerType = Pointer::None;
    public:
        virtual InstRet_T process() = 0;
        bool isImmed8(const QString&);
        bool isChar(const QString&);
        void hexValidator(QStringList&);
        bool isImmed16(const QString&);
        bool isHexValue(const QString&);
        QString extractDisplacment(const QString&, bool *ok = nullptr);
        QString stripSegmentPrefix(const QString&);
        bool hasSegmentPrefix(const QString&);
        QString getSegmentPrefix(const QString&);
        bool isMemAddr(const QString&);
        enum OperandType getOperandType(const QString&);
        void segmentPrefixWrapper(QString&, QString&, QString&);
        void segmentPrefixWrapper(QString&, QString&);
        std::optional<std::tuple<QString, QString>> twoTokens();
        std::optional<std::tuple<QString, QString, QString>> threeTokens();
        const QString& getCodeLine();
        uchar getSegRegCode(const QString&, bool *ok = nullptr);
        uchar getSegRegPrefix(const QString&, bool *ok = nullptr);
        void setCodeLine(const QString&);
        std::optional<QString> signHandler(const QString&, const OperandType&);
        bool isMod1(const int&);
        bool registerMod(const QString&);
        uchar modRegRmGenerator(const uchar&, const uchar&, const uchar&);
        uchar rmGenerator(const QString&);
        virtual uchar getOpcode(const QString&, bool *ok = nullptr)=0;
        uchar getGpRegCode(const QString&, OperandType, bool *ok = nullptr);
        virtual ~Base() = default;
};

#endif
