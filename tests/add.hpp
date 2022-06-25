#include "utmacros.h"
#include <assembler/include/add.h>

struct AddFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new Add;
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(AddFixture, REG8_REG8) {
    strVec code {
    "add ah, al",
    "add bl, ah",
    "add bh, ch",
    "add dH, Ah",
    "add ch, DL",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, MEM8_REG8) {
    strVec code {
    "add bptr [0x11], ah",
    "add [bx+si], ch",
    "add bptr [bx+si+0x11], al",
    "add [si+bp], dl",
    "add wptr [bx+si+0x11], al",
    "add [bx+bp], 0x11",
    "add [si+di], cl",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, REG16_REG16) {
    strVec code {
    "add ax, bx",
    "add bX, Ax",
    "add DX, AX",
    "add cx, dx",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, MEM16_REG16) {
    strVec code {
    "add [0x11], ax",
    "add [bx+si], cx",
    "add [bx+si+0xfd], bx",
    "add [bp+si+0xdd], dx",
    "add wptr [0xbf], bx",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, REG8_MEM8) {
    strVec code {
    "add ah, bptr [bx+si]",
    "add ah, wptr [bx+si]",
    "add ch, [0x1f]",
    "add ch, bptr [0x1f]",
    "add dl, [bx+0xfb]",
    "add bl, [bx+si+0xcf]",
    "add al, [si+bp+0x2f]",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, REG16_MEM16) {
    strVec code {
    "add ax, wptr [0x11]",
    "add ax, [0x11]",
    "add ax, bptr [0x11]",
    "add bx, [bx+si+0xff]",
    "add cx, wptr [si+bp]",
    "add dx, [si+bp+0xffff]",
    "add ax, [si+bp+0xffff]",
    "add cx, wptr [bx+si+0xfff]",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, REG8_IMMED8) {
    strVec code {
    "add al, 0x05",
    "add bl, 0x55",
    "add cl, 0xff",
    "add bh, 0x23",
    "add dh, 0x55",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, MEM8_IMMED8) {
    strVec code {
    /* "add bptr [0xab], 0xff32", */
    //error, should assemble
    "add bptr [0xab], 0xff",
    "add bptr [0xffff], 0x12",
    "add bptr [0x2d4b], 0x22",
    "add bptr [bx+si+0x22], 0x44",
    "add bptr[bp+di],0x24",
    "add bptr [bp+si+0xf24a], 0x21",
    "add [bp+si], 0x22",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, REG16_IMMED16) {
    strVec code {
    "add ax, 0x543",
    "add bx, 0x23ff",
    "add cx, 0x2312",
    "add dx, 0x1",
    "add cs, 0x10",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, MEM16_IMMED16) {
    strVec code {
    "add wptr [0xf1], 0x32",
    "add wptr [bx+0x11], 0xffa2",
    "add wptr [bp+si], 0xffff",
    "add wptr [bp+di+0xf45b], 0x3fe1",
    "add wptr [bx+si+0x3], 0x23df",
    "add wptr [bx+si+0x232], 0xf1e",
    };
    PERFORM_TEST;
}

TEST_F(AddFixture, MISC) {
    strVec code {
        "add di, [bx]",
        "add si, [ax]",
    };
    PERFORM_TEST;
}
