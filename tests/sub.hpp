#include "utmacros.h"
#include <assembler/include/sub.h>

struct SubFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new Sub;
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(SubFixture, REG8_REG8) {
    strVec code {
    "sub ah, al",
    "sub bl, ah",
    "sub bh, ch",
    "sub dH, Ah",
    "sub ch, DL",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, MEM8_REG8) {
    strVec code {
    "sub bptr [0x11], ah",
    "sub [bx+si], ch",
    "sub bptr [bx+si+0x11], al",
    "sub [si+bp], dl",
    "sub wptr [bx+si+0x11], al",
    "sub [bx+bp], 0x11",
    "sub [si+di], cl",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, REG16_REG16) {
    strVec code {
    "sub ax, bx",
    "sub bX, Ax",
    "sub DX, AX",
    "sub cx, dx",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, MEM16_REG16) {
    strVec code {
    "sub [0x11], ax",
    "sub [bx+si], cx",
    "sub [bx+si+0xfd], bx",
    "sub [bp+si+0xdd], dx",
    "sub wptr [0xbf], bx",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, REG8_MEM8) {
    strVec code {
    "sub ah, bptr [bx+si]",
    "sub ah, wptr [bx+si]",
    "sub ch, [0x1f]",
    "sub ch, bptr [0x1f]",
    "sub dl, [bx+0xfb]",
    "sub bl, [bx+si+0xcf]",
    "sub al, [si+bp+0x2f]",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, REG16_MEM16) {
    strVec code {
    "sub ax, wptr [0x11]",
    "sub ax, [0x11]",
    "sub ax, bptr [0x11]",
    "sub bx, [bx+si+0xff]",
    "sub cx, wptr [si+bp]",
    "sub dx, [si+bp+0xffff]",
    "sub ax, [si+bp+0xffff]",
    "sub cx, wptr [bx+si+0xfff]",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, REG8_IMMED8) {
    strVec code {
    "sub al, 0x05",
    "sub bl, 0x55",
    "sub cl, 0xff",
    "sub bh, 0x23",
    "sub dh, 0x55",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, MEM8_IMMED8) {
    strVec code {
    "sub bptr [0xab], 0xff32",
    "sub bptr [0xab], 0xff",
    "sub bptr [0xffff], 0x12",
    "sub bptr [0x2d4b], 0x22",
    "sub bptr [bx+si+0x22], 0x44",
    "sub bptr [bp+di],0x24",
    "sub bptr [bp+si+0xf24a], 0x21",
    "sub [bp+si], 0x22",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, REG16_IMMED16) {
    strVec code {
    "sub ax, 0x543",
    "sub bx, 0x23ff",
    "sub cx, 0x2312",
    "sub dx, 0x1",
    "sub cs, 0x10",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, MEM16_IMMED16) {
    strVec code {
    "sub wptr [0xf1], 0x32",
    "sub wptr [bx+0x11], 0xffa2",
    "sub wptr [bp+si], 0xffff",
    "sub wptr [bp+di+0xf45b], 0x3fe1",
    "sub wptr [bx+si+0x3], 0x23df",
    "sub wptr [bx+si+0x232], 0xf1e",
    };
    PERFORM_TEST;
}

TEST_F(SubFixture, MISC) {
    strVec code {
        "sub di, [bx]",
        "sub si, [ax]",
    };
    PERFORM_TEST;
}
