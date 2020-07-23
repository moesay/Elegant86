#ifndef FPASM_H
#define FPASM_H

#include <QList>
#include <type_traits>
#include "../base.h"

using error = std::tuple<QString, QString, int>;
using line  = std::tuple<QString, OperandType, OperandType>;

const static std::list<line> operandsLUT {

    {"MOV", Reg16, Reg16}, {"MOV", Reg8, Reg8}, {"MOV", Reg16, Reg8}, {"MOV", Reg16, Immed16}, {"MOV", Reg16, Immed8},
    {"MOV", Reg8, Immed8}, {"MOV", Reg16, MemAddr}, {"MOV", Reg8, MemAddr}, {"MOV", MemAddr, Immed16}, {"MOV", MemAddr, Immed8},
    {"MOV", MemAddr, Reg16}, {"MOV", MemAddr, Reg8}, {"MOV", MemAddr, SegReg}, {"MOV", SegReg, MemAddr}, {"MOV", SegReg, Reg16},
    {"MOV", Reg16, SegReg}, /*{"MOV", Reg16, NegImmed16}, {"MOV", Reg16, NegImmed8}, {"MOV", Reg8, NegImmed8},*/

    {"ADD", Reg16, Reg16}, {"ADD", Reg8, Reg8}, {"ADD", Reg16, Reg8}, {"ADD", Reg16, Immed16}, {"ADD", Reg16, Immed8},
    {"ADD", Reg8, Immed8}, {"ADD", Reg16, MemAddr}, {"ADD", Reg8, MemAddr}, {"ADD", MemAddr, Reg16}, {"ADD", MemAddr, Reg8},
    /*{"ADD", Reg16, NegImmed16}, {"ADD", Reg16, NegImmed8}, {"ADD", Reg8, NegImmed8},*/

    {"SUB", Reg16, Reg16}, {"SUB", Reg8, Reg8}, {"SUB", Reg16, Reg8}, {"SUB", Reg16, Immed16}, {"SUB", Reg16, Immed8},
    {"SUB", Reg8, Immed8}, {"SUB", Reg16, MemAddr}, {"SUB", Reg8, MemAddr}, {"SUB", MemAddr, Reg16}, {"SUB", MemAddr, Reg8},
    /*{"SUB", Reg16, NegImmed16}, {"SUB", Reg16, NegImmed8}, {"SUB", Reg8, NegImmed8},*/

    {"PUSH", Reg16, NOP}, {"PUSH", SegReg, NOP}, {"PUSH", MemAddr, NOP},
    {"POP", Reg16, NOP}, {"POP", SegReg, NOP}, {"POP", MemAddr, NOP},

    {"INC", Reg16, NOP}, {"INC", Reg8, NOP}, {"INC", MemAddr, NOP},
    {"DEC", Reg16, NOP}, {"DEC", Reg8, NOP}, {"DEC", MemAddr, NOP},

    {"MUL", Reg8, NOP}, {"MUL", Reg16, NOP}, {"MUL", MemAddr, NOP},
    {"DIV", Reg8, NOP}, {"DIV", Reg16, NOP}, {"DIV", MemAddr, NOP},

    {"IMUL", Reg8, NOP}, {"IMUL", Reg16, NOP}, {"IMUL", MemAddr, NOP},
    {"IDIV", Reg8, NOP}, {"IDIV", Reg16, NOP}, {"IDIV", MemAddr, NOP},

    {"AND", Reg16, Reg16}, {"AND", Reg8, Reg8}, {"AND", Reg16, MemAddr}, {"AND", Reg8, MemAddr},
    {"AND", MemAddr, Reg16}, {"AND", MemAddr, Reg8}, {"AND", Reg16, Immed16}, {"AND", Reg16, Immed8},
    {"AND", Reg8, Immed8}, {"AND", MemAddr, Immed8}, {"AND", MemAddr, Immed16},

    {"OR", Reg16, Reg16}, {"OR", Reg8, Reg8}, {"AND", Reg16, MemAddr}, {"OR", Reg8, MemAddr},
    {"OR", MemAddr, Reg16}, {"OR", MemAddr, Reg8}, {"OR", Reg16, Immed16}, {"OR", Reg16, Immed8},
    {"OR", Reg8, Immed8}, {"OR", MemAddr, Immed8}, {"OR", MemAddr, Immed16},

    {"XOR", Reg16, Reg16}, {"XOR", Reg8, Reg8}, {"AND", Reg16, MemAddr}, {"XOR", Reg8, MemAddr},
    {"XOR", MemAddr, Reg16}, {"XOR", MemAddr, Reg8}, {"XOR", Reg16, Immed16}, {"XOR", Reg16, Immed8},
    {"XOR", Reg8, Immed8}, {"XOR", MemAddr, Immed8}, {"XOR", MemAddr, Immed16},

    {"SHR", Immed8, NOP}, {"SHR", Reg8, NOP},
    {"SHL", Immed8, NOP}, {"SHL", Reg8, NOP},

    {"JMP", Reg16, NOP}, {"JMP", MemAddr, NOP}, {"JMP", Label, NOP},
    {"CLC", NOP, NOP},
};

const static std::list<QString> instructionsLUT {
        "MOV"  ,"PUSH" , "POP" , "XCHG","IN"  ,"OUT" ,
        "XLAT" ,"LEA"  , "LDS" , "LES" ,"LAHF","SHAF",
        "PUSHF","POPF" , "ADD" , "ADC" ,"INC" ,"AAA" ,
        "DAA"  ,"SUB"  , "SBB" , "DES" ,"NEG" ,"CMP" ,
        "AAS"  ,"DAS"  , "MUL" , "IMUL","AAM" ,"DIV" ,
        "IDIV" ,"AAD"  , "CBW" , "CWD" ,"NOT" ,"SHL" ,
        "SAL"  ,"SHR"  , "ROL" , "ROR" ,"RCL" ,"RCR" ,
        "AND"  ,"TEST" , "OR"  , "XOR" ,"REP" ,"MOVS",
        "CMPS" ,"SCAS" , "LODS", "STDS","CALL","JMP" ,
        "RET"  ,"JE"   , "JZ"  , "JL"  ,"JNGE","JLE" ,
        "JNG"  ,"JB"   , "JNAE", "JBE" ,"JNA" ,"JP"  ,
        "JPE"  ,"JO"   , "JS"  , "JNE" ,"JNZ" ,"JNL" ,
        "JGE"  ,"JNLE" , "JG"  , "JNB" ,"JAE" ,"JNBE",
        "JA"   ,"JNP"  , "JPO" , "JNO" ,"JNS" ,"LOOP",
        "LOOPZ","LOOPE", "JCXZ", "INT" ,"INTO","IRET",
        "CLC"  ,"CMC"  , "STC" , "CLD" ,"STD" ,"CLI" ,
        "STI"  ,"HLT"  , "WAIT", "ESC" ,"LOCK"
};

class FirstPass : public Base {
    public:
        FirstPass() {};
        ~FirstPass() {};
        QString process() {return "";};
        uchar getOpcode(const QString&, [[maybe_unused]] bool *ok = nullptr) {return 0x00;};

    /* private: */
        QStringList readyCode;
        void Iprocess (QStringList& /*code*/, QStringList& /*variables*/, QStringList& /*labels*/);
        QList<label> getLabels(const QStringList&);
        QList<error> validate(const QStringList&);
        void negativeSignProcess(QStringList&);
        void removeComments(QStringList&);
        QString eval(const QString&);
};

#endif
