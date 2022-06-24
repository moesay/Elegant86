#include "utmacros.h"
#include <assembler/include/pop.h>

struct PopFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new Pop();
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(PopFixture, SegRegs) {
    strVec code {
    "pop ES",
    "pop cs",
    "pop ss",
    "pop dS",
    };
    PERFORM_TEST;
}

TEST_F(PopFixture, GpRegs) {
    strVec code {
    "pop aX",
    "pop bx",
    "pop cx",
    "pop dx",
    "pop ah",
    "pop ch",
    "pop wptr ax",
    };
    PERFORM_TEST;
}

TEST_F(PopFixture, MemAddrs) {
    strVec code {
    "pop wptr [0xff]",
    "pop wptr [0x43]",
    "pop bptr [0xff]",
    };
    PERFORM_TEST;
}

TEST_F(PopFixture, Indexers) {
    strVec code {
    "pop sp",
    "pop bp",
    "pop si",
    "pop di",
    };
    PERFORM_TEST;
}
