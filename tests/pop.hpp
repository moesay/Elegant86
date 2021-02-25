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
    SET("pop ES");
    T("07");

    SET("pop cs");
    T("");

    SET("pop ss");
    T("17");

    SET("pop dS");
    T("1F");
}

TEST_F(PopFixture, GpRegs) {
    SET("pop aX");
    T("58");

    SET("pop bx");
    T("5B");

    SET("pop cx");
    T("59");

    SET("pop dx");
    T("5A");

    SET("pop ah");
    T("");

    SET("pop ch");
    T("");

    SET("pop wptr ax");
    T("58");
}

TEST_F(PopFixture, MemAddrs) {
    SET("pop wptr [ff]");
    T("8F06FF00");

    SET("pop wptr [43]");
    T("8F064300");

    SET("pop bptr [ff]");
    T("");
}

TEST_F(PopFixture, Indexers) {
    SET("pop sp");
    T("5C");

    SET("pop bp");
    T("5D");

    SET("pop si");
    T("5E");

    SET("pop di");
    T("5F");
}
