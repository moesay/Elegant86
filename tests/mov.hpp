#include "utmacros.h"
#include <assembler/include/mov.h>

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
    SET("mov bptr [bx+11], 44");
    auto [v1, v2, v3] = b->threeTokens();
    EXPECT_STRCASEEQ(v1.toStdString().c_str(), "mov");
    EXPECT_STRCASEEQ(v2.toStdString().c_str(), "[bx+11]");
    EXPECT_STREQ(v3.toStdString().c_str(), "44");
}

TEST_F(MovFixture, REG8_REG8) {
    SET("mov ah, al");
    T("88C4");

    SET("mov bl, ah");
    T("88E3");

    SET("mov bh, ch");
    T("88EF");

    SET("mov dH, Ah");
    T("88E6");

    SET("mov ch, DL");
    T("88D5");
}

TEST_F(MovFixture, MEM8_REG8) {
    SET("mov bptr [11], ah");
    T("88261100");

    SET("mov [bx+si], ch");
    T("8828");

    SET("mov bptr [bx+si+11], al");
    T("884011");

    SET("mov wptr [bx+si+11], al");
    T("");

    SET("mov [bx+bp], 11");
    T("");

    SET("mov [si+di], cl");
    T("");

    SET("mov [si+bp], dl");
    T("8812");
}

TEST_F(MovFixture, REG16_REG16) {
    SET("mov ax, bx");
    T("89D8");

    SET("mOv bX, Ax");
    T("89C3");

    SET("MOV DX, AX");
    T("89C2");

    SET("mov cx, dx");
    T("89D1");
}

TEST_F(MovFixture, MEM16_REG16) {
    SET("mov [11], ax");
    T("A31100");

    SET("mov [bx+si], cx");
    T("8908");

    SET("mov [bx+si+fd], bx");
    T("8998FD00");

    SET("mov [bp+si+dd], dx");
    T("8992DD00");

    SET("mov wptr [bf], bx");
    T("891EBF00");
}

TEST_F(MovFixture, REG8_MEM8) {
    SET("mov ah, bptr [bx+si]");
    T("8A20");

    SET("mov ah, [1]");
    T("8A260100");

    SET("mov ah, wptr [bx+si]");
    T("");

    SET("mov ch, [1f]");
    T("8A2E1F00");

    SET("mov ch, bptr [1f]");
    T("8A2E1F00");

    SET("mov dl, [bx+fb]");
    T("8A97FB00");

    SET("mov bl, [bx+si+cf]");
    T("8A98CF00");

    SET("mov al, [si+bp+2f]");
    T("8A422F");
}

TEST_F(MovFixture, REG16_MEM16) {
    SET("mov ax, wptr [11]");
    T("A11100");

    SET("mov ax, [11]");
    T("A11100");

    SET("mov bx, [2]");
    T("8B1E0200");

    SET("mov ax, [11f]");
    T("A11F01");

    SET("mov ax, bptr [11]");
    T("");

    SET("MoV bx, [bx+si+ff]");
    T("8B98FF00");

    SET("mov cx, wptr [si+bp]");
    T("8B0A");

    SET("mov dx, [si+bp+ffff]");
    T("8B92FFFF");

    SET("mov ax, [si+bp+ffff]");
    T("8B82FFFF");

    SET("mov cx, wptr [bx+si+fff]");
    T("8B88FF0F");
}

TEST_F(MovFixture, MEM8_IMMED8) {
    SET("mov bptr [ab], ff32");
    T("");

    SET("mov bptr [ab], ff");
    T("C606AB00FF");

    SET("mov bptr [ffff], 12");
    T("C606FFFF12");

    SET("mov bptr [2d4b], 22");
    T("C6064B2D22");

    SET("mov bptr [bx+si+22], 44");
    T("C6402244");

    SET("mov bptr[bp+di],24");
    T("C60324");

    SET("mov bptr [bp+si+f24a], 21");
    T("C6824AF221");

    SET("mov [bp+si], 22");
    T("");
}

TEST_F(MovFixture, MEM16_IMMED16) {
    SET("mov wptr [f1], 32");
    T("C706F1003200");

    SET("mov wptr [bx+11], ffa2");
    T("C74711A2FF");

    SET("mov wptr [bp+si], ffff");
    T("C702FFFF");

    SET("mov wptr [bp+di+f45b], 3fe1");
    T("C7835BF4E13F");

    SET("mov wptr [bx+si+3], 23df");
    T("C74003DF23");

    SET("mov wptr [bx+si+232], f1e");
    T("C78032021E0F");
}

TEST_F(MovFixture, REG16_SEGREG) {
    SET("mov ax, ds");
    T("8CD8");

    SET("mov ds, ax");
    T("8ED8");

    SET("mov bx, ss");
    T("8CD3");

    SET("mov ax, ss");
    T("8CD0");

    SET("mov bx, ds");
    T("8CDB");

    SET("mov cx, es");
    T("8CC1");

    SET("mov ax, es");
    T("8CC0");

    SET("mov dx, cs");
    T("8CCA");
}

TEST_F(MovFixture, MEM16_SEGREG) {
    SET("mov [22], ds");
    T("8C1E2200");

    SET("mov [bx+si], ss");
    T("8C10");

    SET("mov wptr [bp+si+2], es");
    T("8C4202");

    SET("mov [bx], ds");
    T("8C1F");

    SET("mov [bp+di+33f],cs");
    T("8C8B3F03");

    SET("mov [abf], cs");
    T("8C0EBF0A");

    SET("mov bptr [bx+si], ss");
    T("");
}

TEST_F(MovFixture, SEGREG_MEM16) {
    SET("mov ss, [11]");
    T("8E161100");

    SET("mov wptr 11, ax");
    T("");

    SET("mov wptr ss, [11]");
    T("8E161100");

    SET("mov ss, bptr [11]");
    T("");

    SET("MOV ES, [BX+SI]");
    T("8E00");

    SET("mov ss, [bx]");
    T("8E17");

    SET("mov cs, [si+bp]");
    T("8E0A");

    SET("mov es, [bp+si+2fd]");
    T("8E82FD02");

    SET("mov cs, [bx+241]");
    T("8E8F4102");
}

TEST_F(MovFixture, SEGREG_REG16) {
    SET("mov es, ax");
    T("8EC0");

    SET("mov ds, bx");
    T("8EDB");

    SET("mov ss, cx");
    T("8ED1");

    SET("mov cs, ax");
    T("8EC8");

    SET("mov ss, ax");
    T("8ED0");
}

TEST_F(MovFixture, ACCUM) {
    SET("mov al, [1]");
    T("A00100");

    SET("mov al, [bx+si]");
    T("8A00");

    SET("mov al, [bx]");
    T("8A07");

    SET("mov al, [bx+si+2311]");
    T("8A801123");

    SET("mov al, [bp+di+34]");
    T("8A4334");

    SET("mov ax, [1]");
    T("A10100");

    SET("mov ax, [bx+si]");
    T("8B00");

    SET("mov ax, [bx]");
    T("8B07");

    SET("mov ax, [bx+si+2311]");
    T("8B801123");

    SET("mov ax, [bp+di+34]");
    T("8B4334");

    SET("mov al, [11111111]");
    T("A01111");

    SET("mov al, 5");
    T("B005");
}

TEST_F(MovFixture, IMMED) {
    SET("mov al, 1");
    T("B001");

    SET("mov al, 23");
    T("B023");

    SET("mov al, 111");
    T("");

    SET("mov ah, 1");
    T("B401");

    SET("mov ah, 23");
    T("B423");

    SET("mov ah, 111");
    T("");

    SET("mov bl, 1");
    T("B301");

    SET("mov bl, 23");
    T("B323");

    SET("mov bl, 111");
    T("");

    SET("mov bh, 1");
    T("B701");

    SET("mov bh, 23");
    T("B723");

    SET("mov bh, 111");
    T("");

    SET("mov cl, 1");
    T("B101");

    SET("mov cl, 23");
    T("B123");

    SET("mov cl, 111");
    T("");

    SET("mov ch, 1");
    T("B501");

    SET("mov ch, 23");
    T("B523");

    SET("mov ch, 111");
    T("");

    SET("mov dl, 1");
    T("B201");

    SET("mov dl, 23");
    T("B223");

    SET("mov dl, 111");
    T("");

    SET("mov dh, 1");
    T("B601");

    SET("mov dh, 23");
    T("B623");

    SET("mov dh, 111");
    T("");
}

TEST_F(MovFixture, SEGMENT_OVERRIDE) {
    SET("mov ds:[11], ax");
    T("3EA31100");

    SET("mov cs:[bx+si+22], bx");
    T("2E895822");

    SET("mov cx, ds:[bx]");
    T("3E8B0F");

    SET("mov ch, ds:[bx]");
    T("3E8A2F");

    SET("mov wptr [bx+si+ff], ds:[11]");
    T("");

    SET("mov wptr [bx+si+ff], ds:[bx]");
    T("");
}

TEST_F(MovFixture, INDEXERS) {
    SET("mov ax, si");
    T("89F0");

    SET("mov si, bx");
    T("89DE");

    SET("mov si, [bx+si+22]");
    T("8B7022");

    SET("mov si, [bx+si]");
    T("8B30");

    SET("mov di, ax");
    T("89C7");

    SET("mov di, [bx]");
    T("8B3F");

    SET("mov bp, ax");
    T("89C5");

    SET("mov sp, [11]");
    T("8B261100");

    SET("mov sp, dx");
    T("89D4");
}
