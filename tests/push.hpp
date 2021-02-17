#include "utmacros.h"
#include <assembler/include/push.h>

struct PushFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new Push();
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(PushFixture, SegRegs) {
    SET("push ES");
    T("06");

    SET("push cs");
    T("0E");

    SET("pUsH ss");
    T("16");

    SET("push dS");
    T("1E");
}

TEST_F(PushFixture, GpRegs) {
    SET("push aX");
    T("50");

    SET("push bx");
    T("53");

    SET("push cx");
    T("51");

    SET("push dx");
    T("52");

    SET("push ah");
    T("");

    SET("push ch");
    T("");

    SET("push wptr ax");
    T("50");
}

TEST_F(PushFixture, MemAddrs) {
    SET("push wptr [ff]");
    T("FF36FF00");

    SET("push wptr [43]");
    T("ff364300");

    SET("PUSH bptr [ff]");
    T("");
}

TEST_F(PushFixture, Indexers) {
    SET("push sp");
    T("54");

    SET("push bp");
    T("55");

    SET("push si");
    T("56");

    SET("push di");
    T("57");
}
