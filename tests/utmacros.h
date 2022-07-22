#ifndef UTMACROS_H
#define UTMACROS_H

#include <assembler/include/base.h>
#include <gtest/gtest.h>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#define BUFFER 256

using strVec = std::vector<std::string>;

strVec loadAndTest(std::string path) {
    std::ifstream fileS(path);
    strVec code;
    std::string temp;
    while(std::getline(fileS, temp))
        code.push_back(temp);

    for(auto x: code)
        std::cout << x << "\n";

    fileS.close();
    return code;
}

std::string decode (std::string machCode) {
    std::string cmd = "rasm2 -a x86.as -b 16 -d '" + machCode + "'";
    std::string output;
    char buffer[BUFFER];
    FILE* pipe;

    pipe = popen(cmd.c_str(), "r");
    if(pipe) {
        while(!feof(pipe)) {
            if(fgets(buffer, BUFFER, pipe) != NULL) {
                if(buffer[strlen(buffer) - 1] == '\n')
                    buffer[strlen(buffer) -1] = '\0';
                output.append(buffer);
            }
        }
        pclose(pipe);
    }
    return output;
}

std::string encode(std::string asmCode) {
    asmCode = std::regex_replace(asmCode, std::regex("wptr"), "word ptr");
    asmCode = std::regex_replace(asmCode, std::regex("bptr"), "byte ptr");
    std::string output;
    std::string cmd = "rasm2 -a x86.as -b 16 -s intel '" + asmCode + "'";
    FILE * pipe;
    char buffer[BUFFER];

    pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        while (!feof(pipe)) {
            if (fgets(buffer, BUFFER, pipe) != NULL) {
                if(buffer[strlen(buffer) - 1] == '\n')
                    buffer[strlen(buffer) -1] = '\0';
                output.append(buffer);
            }
        }
        pclose(pipe);
    }
    return output;
}
#define ELE_OP std::get<0>(b->process()).toStdString()
#define T(x) EXPECT_STRCASEEQ(                                                      \
        encode(                                                                     \
                decode(ELE_OP)).c_str()                                             \
                , x)                                                                \
    << "\nAssembly Code : " << b->getCodeLine().toStdString()                       \
    << "\n-----------------------------------------"

#define SET(x) b->setCodeLine(x)

#define PERFORM_TEST                                        \
    for(const auto& line : code){                           \
        SET(line);                                          \
        T(encode(line).c_str());                            \
    }

#endif
