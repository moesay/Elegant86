#include "utmacros.h"
#include <assembler/include/mov.h>
#include <cstring>
#include <string>
#include <vector>

struct MovFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new Mov();
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(MovFixture, UTILS) {
    SET(std::string("mov bptr [bx+11], 0x44"));
    auto [v1, v2, v3] = b->tokenize().value_or(std::make_tuple("", "", ""));
    EXPECT_STRCASEEQ(v1.toStdString().c_str(), "mov");
    EXPECT_STRCASEEQ(v2.toStdString().c_str(), "[bx+11]");
    EXPECT_STREQ(v3.toStdString().c_str(), "0X44");
}

TEST_F(MovFixture, REG8_REG8) {
    strVec code
    {
        "mov ah, al",
            "mov bl, ah",
            "mov bh, ch",
            "mov dH, Ah",
            "mov ch, DL"
    };

    PERFORM_TEST;
}

TEST_F(MovFixture, MEM8_REG8) {
    strVec code {
        "mov bptr [0x11], ah",
            "mov [bx+si], ch",
            "mov bptr [bx+si+0x11], al",
            "mov bptr [si+bp], dl",
        "mov wptr [bx+si+0x11], al",
            "mov [bx+bp], 0x11",
            "mov [si+di], cl",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, REG16_REG16) {
    strVec code {
        "mov ax, bx",
            "mOv bX, Ax",
            "MOV DX, AX",
            "mov cx, dx",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, MEM16_REG16) {

    strVec code {
        "mov [0x11], ax",
            "mov [bx+si], cx",
            "mov [bx+si+0xfd], bx",
            "mov [bp+si+0xdd], dx",
            "mov wptr [0xbf], bx",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, REG8_MEM8) {
    strVec code {
        "mov ah, bptr [bx+si]",
            "mov ah, [0x1]",
            "mov ch, [0x1f]",
            "mov ch, bptr [0x1f]",
            "mov dl, [bx+0xfb]",
            "mov bl, [bx+si+0xcf]",
            "mov al, [si+bp+0x2f]",
        "mov ah, wptr [bx+si]"
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, REG16_MEM16) {
    strVec code {
        "mov ax, wptr [0x11]",
            "mov ax, [0x11]",
            "mov bx, [0x2]",
            "mov ax, [0x11f]",
            "MoV bx, [bx+si+0xff]",
            "mov cx, wptr [si+bp]",
            "mov dx, [si+bp+0xffff]",
            "mov ax, [si+bp+0xffff]",
            "mov cx, wptr [bx+si+0xfff]",
        "mov ax, bptr [0x11]"
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, MEM8_IMMED8) {
    strVec code {
        "mov bptr [0xab], 0xff",
        "mov bptr [0xffff], 0x12",
            "mov bptr [0x2d4b], 0x22",
            "mov [bp+si], 0x22",
            "mov bptr [bx+si+0x22], 0x44",
            /* "mov bptr [bx+si+9], 0100101b", --> only supported by Elegant86*/
            "mov bptr [bp+di],0x24",
            "mov bptr [bp+si+0xf24a], 0x21",
            "mov bptr [0xab], 0xff32",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, MEM16_IMMED16) {
    strVec code {
        "mov wptr [0xf1], 0x32",
            "mov wptr [bx+0x11], 0xffa2",
            "mov wptr [bp+si], 0xffff",
            "mov wptr [bp+di+0xf45b], 0x3fe1",
            "mov wptr [bx+si+0x3], 0x23df",
            "mov wptr [bx+si+0x232], 0xf1e"
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, REG16_SEGREG) {
    strVec code {
        "mov ax, ds",
            "mov ds, ax",
            "mov bx, ss",
            "mov ax, ss",
            "mov bx, ds",
            "mov cx, es",
            "mov ax, es",
            "mov dx, cs",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, MEM16_SEGREG) {
    strVec code {
        "mov [0x22], ds",
            "mov [bx+si], ss",
            "mov wptr [bp+si+0x2], es",
            "mov [bx], ds",
            "mov [bp+di+0x33f],cs",
        "mov bptr [bx+si], ss",
            "mov [0xabf], cs",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, SEGREG_MEM16) {
    strVec code {
        "mov ss, [0x11]",
            "mov ss, wptr [0x11]",
            "MOV ES, [BX+SI]",
            "mov ss, [bx]",
            "mov cs, [si+bp]",
            "mov es, [bp+si+0x2fd]",
        "mov wptr 0x11, ax"
            "mov ss, bptr [0x11]"
            "mov cs, [bx+0x241]",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, SEGREG_REG16) {
    strVec code {
        "mov es, ax",
            "mov ds, bx",
            "mov ss, cx",
            "mov cs, ax",
            "mov ss, ax",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, ACCUM) {
    strVec code {
        "mov al, [0x1]",
            "mov ax, 11",
            "mov al, [bx+si]",
            "mov al, [bx]",
            "mov al, [bx+si+0x2311]",
            "mov al, [bp+di+0x34]",
            "mov ax, [0x1]",
            "mov ax, [bx+si]",
            "mov ax, [bx]",
            "mov ax, [bx+si+0x2311]",
            "mov ax, [bp+di+0x34]",
            "mov al, [0x11111111]",
            "mov al, 0x5",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, IMMED) {
    strVec code {
        "mov al, 0x111",
            "mov ah, 0x111",
            "mov bl, 0x111",
            "mov bh, 0x111",
            "mov cl, 0x111",
            "mov al, 0x1",
            "mov al, 0x23",
            "mov ah, 0x1",
            "mov ah, 0x23",
            "mov bl, 0x1",
            "mov bl, 0x23",
            "mov bh, 0x1",
            "mov bh, 0x29",
            "mov cl, 0x1",
            "mov cl, 0x23",
            "mov ch, 0x1",
            "mov ch, 0x23",
            "mov ch, 0x111",
            "mov dl, 0x1",
            "mov dl, 0x23",
            "mov dl, 0x111",
            "mov dh, 0x1",
            "mov dh, 0x23",
            "mov dh, 0x111",
            "mov ah, 0xabcd1123"
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, SEGMENT_OVERRIDE) {
    strVec code {
    "mov ds:[0x11], ax",
    "mov cs:[bx+si+0x22], bx",
    "mov cx, ds:[bx]",
    "mov ch, ds:[bx]",
    "mov wptr [bx+si+0xff], ds:[0x11]",
    "mov wptr [bx+si+0xff], ds:[bx]",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, INDEXERS) {
    strVec code {
    "mov ax, si",
    "mov si, bx",
    "mov si, [bx+si+0x22]",
    "mov si, [bx+si]",
    "mov di, ax",
    "mov di, [bx]",
    "mov bp, ax",
    "mov sp, [0x11]",
    "mov sp, dx",
    };
    PERFORM_TEST;
}

TEST_F(MovFixture, FROM_FILE) {
    strVec code = loadAndTest("./testCode/mov.asm");
    PERFORM_TEST;
}
