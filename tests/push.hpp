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

    strVec code {
    "push ES",
    "push cs",
    "pUsH ss",
    "push dS",
    };
    PERFORM_TEST;
}

TEST_F(PushFixture, GpRegs) {
    strVec code {
    "push aX",
    "push bx",
    "push cx",
    "push dx",
    "push ah",
    "push ch",
    "push wptr ax",
    };
    PERFORM_TEST;
}
TEST_F(PushFixture, MemAddrs) {
    strVec code {
    "push wptr [0xff]",
    "push wptr [0x43]",
    "PUSH bptr [0xff]",
    };
    PERFORM_TEST;
}

TEST_F(PushFixture, Indexers) {
    strVec code {
    "push sp",
    "push bp",
    "push si",
    "push di",
    };
    PERFORM_TEST;
}
