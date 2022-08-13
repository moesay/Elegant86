#ifndef JMPS_H
#define JMPS_H

#include "base.h"

class Jmps : public Base {
public:
    Jmps(const QString&);
    Jmps();
    InstRet_T process() override;
    uchar getOpcode(const QString&, bool* ok = nullptr) override;
    QString negate(const QString&);

private:
    const std::vector<std::string> neg {
        "JO", "JNO", "JB", "JNB", "JAE", "JNAE", "JC", "JNC",
        "JE", "JNE", "JZ", "JNZ", "JBE", "JNBE", "JA", "JNA",
        "JS", "JNS", "JP", "JNP", "JPE", "JPO", "JL", "JNL",
        "JGE", "JGE", "JNGE", "JLE", "JNLE", "JG", "JNG" };

    const std::unordered_map<std::string, uchar> LUT{
        {"JO", 0x70}, {"JNO", 0X71}, {"JB", 0X72}, {"JC", 0X72},
        {"JNAE", 0X72}, {"JNB", 0X73}, {"JAE", 0X73}, {"JNC", 0X73},
        {"JE", 0X74}, {"JZ", 0X74}, {"JNE", 0X75}, {"JNZ", 0X75},
        {"JBE", 0X76}, {"JNA", 0X76}, {"JNBE", 0X77}, {"JA", 0X77},
        {"JS", 0X78}, {"JNS", 0X79}, {"JP", 0X7A}, {"JPE", 0X7A},
        {"JNP", 0X7B}, {"JPO", 0X7B}, {"JL", 0X7C}, {"JNGE", 0X7C},
        {"JNL", 0X7C}, {"JGE", 0X7C}, {"JLE", 0X7D}, {"JNG", 0X7D},
        {"JNLE", 0X7F}, {"JG", 0X7F}
    };
};

#endif
