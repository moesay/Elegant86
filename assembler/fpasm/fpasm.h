#ifndef FPASM_H
#define FPASM_H

#include <assembler/include/base.h>
#include <optional>
#include <assembler/include/precom.h>
#include <include/labels.h>

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

class FirstPhase : public Base {
    public:
        FirstPhase (const FirstPhase&) = delete;
        InstRet_T process() override{return {"", false, ""};};
        uchar getOpcode(const QString&, [[maybe_unused]] bool *ok = nullptr) override {return 0x00;};
        static std::tuple<QStringList, QList<Error_T>> validate (const QStringList&);
        static InstRet_T assemble(const QString&);
    private:
        static FirstPhase& Get() {
            static FirstPhase ref;
            return ref;
        }
        FirstPhase() {};
        QStringList readyCode;
        QList<Error_T> getLabels(QStringList&);
        std::tuple<QStringList, QList<Error_T>> Ivalidate(const QStringList&);
        InstRet_T Iassemble(const QString&);
        void negativeSignProcess(QStringList&);
        void removeComments(QStringList&) noexcept;
        std::optional<QString> eval(QString);
};

#endif
