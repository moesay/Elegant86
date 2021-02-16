#ifndef FPASM_H
#define FPASM_H

#include <QList>
#include <assembler/include/base.h>
#include <assembler/include/precom.h>

using error = std::tuple<QString, QString, int>;
using line  = std::tuple<QString, OperandType, OperandType>;

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
        FirstPass (const FirstPass&) = delete;
        InstRet process() override{return {"", false, ""};};
        uchar getOpcode(const QString&, [[maybe_unused]] bool *ok = nullptr) override {return 0x00;};
        static std::tuple<QStringList, QList<error>> validate (const QStringList&);
        static InstRet assemble(const QString&);
    private:
        static FirstPass& Get() {
            static FirstPass ref;
            return ref;
        }
        FirstPass() {};
        QStringList readyCode;
        bool isLabel(const QString&);
        QList<label> getLabels(QStringList&);
        std::tuple<QStringList, QList<error>> Ivalidate(const QStringList&);
        InstRet Iassemble(const QString&);
        void negativeSignProcess(QStringList&);
        void removeComments(QStringList&) noexcept;
        bool eval(QString&);
};

#endif
