#ifndef FPASM_H
#define FPASM_H

#include <assembler/include/base.h>
#include <assembler/include/precom.h>
#include "labels.h"
#include <string>
#include <vector>

const static std::list<std::string> JMPS {
    "JE"   , "JZ"  , "JL"  ,"JNGE","JLE" , "LOOPZ",
    "JNG"  ,"JB"   , "JNAE", "JBE" ,"JNA" ,"JP"  ,
    "JPE"  ,"JO"   , "JS"  , "JNE" ,"JNZ" ,"JNL" ,
    "JGE"  ,"JNLE" , "JG"  , "JNB" ,"JAE" ,"JNBE",
    "JA"   ,"JNP"  , "JPO" , "JNO" ,"JNS" ,"LOOP",
    "LOOPE", "CALL", "JCXZ",
};

const static std::list<std::string> instructionsLUT {
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

const static std::list<std::string> NoOpInsts{
    "AAA", "AAD", "AAM", "AAS", "CBW", "CLC", "CLD",
    "CLI", "CMC", "CWD", "DAA", "DAS", "HTL", "INTO",
    "IRET", "LAHF", "LOCK", "NOP", "POPF", "PUSHF",
    "SAHF", "STC", "STD", "STI", "WAIT"
};

struct output_t {
    std::string code;
    std::string machCode;
    std::string err;
    int ln;
    bool state;
};

class FirstPhase : public Base {
    public:
        FirstPhase (const FirstPhase&) = delete;
        static void validate (const QStringList&);
        static std::vector<output_t> assemble();
        static std::vector<Error_T> errors;
    private:
        std::vector<info_t> codeInfo;
        InstRet_T process() override{return {"", false, ""};};
        uchar getOpcode(const QString&, [[maybe_unused]] bool *ok = nullptr) override {return 0x00;};
        static FirstPhase& Get() {
            static FirstPhase ref;
            return ref;
        }
        FirstPhase() {};
        void getLabels();
        void Ivalidate(const QStringList&);
        std::vector<output_t> Iassemble();
        InstRet_T preAsm(const QString&, bool fetch = false);
        void negativeSignProcess(QStringList&);
        void removeComments() noexcept;
};

#endif
