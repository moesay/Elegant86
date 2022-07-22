#include "utmacros.h"
#include <assembler/include/and.h>

struct AndFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new And();
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(AndFixture, REG8_REG8) {
    strVec code {
        "and ah, al",
            "and bl, ah",
            "and bh, ch",
            "and dH, Ah",
            "and ch, DL",
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, MEM8_REG8) {
    strVec code {
        "and bptr [0x11], ah",
            "and [bx+si], ch",
            "and bptr [bx+si+0x11], al",
            "and [si+bp], dl",
        "and wptr [bx+si+0x11], al",
            "and [bx+bp], 0x11",
            "and [si+di], cl",
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, REG16_REG16) {
    strVec code {
        "and ax, bx",
            "and bX, Ax",
            "and DX, AX",
            "and cx, dx",
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, MEM16_REG16) {
    strVec code {
        "and [0x11], ax",
            "and [bx+si], cx",
            "and [bx+si+0xfd], bx",
            "and [bp+si+0xdd], dx",
            "and wptr [0xbf], bx",
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, REG8_MEM8) {
    strVec code {
        "and ah, bptr [bx+si]",
            "and ah, [0x1]",
            "and ch, [0x1f]",
            "and ch, bptr [0x1f]",
            "and dl, [bx+0xfb]",
            "and bl, [bx+si+0xcf]",
            "and al, [si+bp+0x2f]",
        "and ah, wptr [bx+si]"
    };

    PERFORM_TEST;

}

TEST_F(AndFixture, REG16_MEM16) {
    strVec code {
        "and ax, wptr [0x11]",
            "and ax, [0x11]",
            "and bx, [0x2]",
            "and ax, [0x11f]",
            "and bx, [bx+si+0xff]",
            "and cx, wptr [si+bp]",
            "and dx, [si+bp+0xffff]",
            "and ax, [si+bp+0xffff]",
            "and cx, wptr [bx+si+0xfff]",
        "and ax, bptr [11]"
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, MEM8_IMMED8) {

    strVec code {
        "and bptr [0xab], 0xff",
            "and bptr [0xffff], 0x12",
            "and bptr [0x2d4b], 0x22",
            "and bptr [bx+si+0x22], 0x44",
            "and bptr [bp+di],0x24",
            "and bptr [bp+si+0xf24a], 0x21",
        "and [bp+si], 0x22",
        "and bptr [0xab], 0xff32",
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, MEM16_IMMED16) {
    strVec code {
        "and wptr [0xf1], 0x32",
            "and wptr [bx+0x11], 0xffa2",
            "and wptr [bp+si], 0xffff",
            "and wptr [bp+di+0xf45b], 0x3fe1",
            "and wptr [bx+si+0x3], 0x23df",
            "and wptr [bx+si+0x232], 0xf1e",
    };
    PERFORM_TEST;
}


TEST_F(AndFixture, ACCUM) {
    strVec code {
    "and al, [0x1]",
    "and al, [bx+si]",
    "and al, [bx]",
    "and al, [bx+si+0x2311]",
    "and al, [bp+di+0x34]",
    "and ax, [0x1]",
    "and ax, [bx+si]",
    "and ax, [bx]",
    "and ax, [bx+si+0x2311]",
    "and ax, [bp+di+0x34]",
    "and al, 0x5",
    "and al, [0x11111111]"
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, IMMED) {
    strVec code {
    "and al, 0x1",
    "and al, 0x23",
    "and ah, 0x1",
    "and ah, 0x23",
    "and bl, 0x1",
    "and bl, 0x23",
    "and bh, 0x1",
    "and bh, 0x23",
    "and cl, 0x1",
    "and cl, 0x23",
    "and ch, 0x1",
    "and ch, 0x23",
    "and dl, 0x1",
    "and dl, 0x23",
    "and dh, 0x1",
    "and dh, 0x23",
    "and al, 111",
    "and ah, 111",
    "and ch, 111",
    "and cl, 111",
    "and bh, 111",
    "and dh, 111",
    "and dl, 111",
    "and bl, 111",
    };
    PERFORM_TEST;

}

TEST_F(AndFixture, SEGMENT_OVERRIDE) {

    strVec code {
    "and ds:[0x11], ax",
    "and cs:[bx+si+0x22], bx",
    "and cx, ds:[bx]",
    "and ch, ds:[bx]",
    "and wptr [bx+si+0xff], ds:[11]",
    "and wptr [bx+si+0xff], ds:[bx]",
    };
    PERFORM_TEST;
}

TEST_F(AndFixture, INDEXERS) {
    strVec code {
    "and ax, si",
    "and si, bx",
    "and si, [bx+si+0x22]",
    "and si, [bx+si]",
    "and di, ax",
    "and di, [bx]",
    "and bp, ax",
    "and sp, [0x11]",
    "and sp, dx",
    };
    PERFORM_TEST;
}
