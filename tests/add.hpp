#include <gtest/gtest.h>
#include <assembler/include/precom.h>

#define T(x) EXPECT_STRCASEEQ(std::get<0>(b->process()).toStdString().c_str(), x);
#define SET(x) b->setCodeLine(x);

struct AddFixture : testing::Test {
    Base *b;
    void SetUp() {
        b = new Add;
    }

    void TearDown() {
        delete b;
    }
};

TEST_F(AddFixture, UTILS) {
    SET("add bptr [bx+11], 44");
    auto [v1, v2, v3] = b->threeTokens();
    EXPECT_STRCASEEQ(v1.toStdString().c_str(), "add");
    EXPECT_STRCASEEQ(v2.toStdString().c_str(), "[bx+11]");
    EXPECT_STREQ(v3.toStdString().c_str(), "44");
}

TEST_F(AddFixture, REG8_REG8) {
    SET("add ah, al")
        T("00C4")

        SET("add bl, ah")
        T("00E3")

        SET("add bh, ch")
        T("00EF")

        SET("add dH, Ah")
        T("00E6")

        SET("add ch, DL")
        T("00D5")
}

TEST_F(AddFixture, MEM8_REG8) {
    SET("add bptr [11], ah")
        T("00261100")

        SET("add [bx+si], ch")
        T("0028")

        SET("add bptr [bx+si+11], al")
        T("004011")

        SET("add wptr [bx+si+11], al")
        T("")

        SET("add [bx+bp], 11")
        T("")

        SET("add [si+di], cl")
        T("")

        SET("add [si+bp], dl")
        T("0012")
}

TEST_F(AddFixture, REG16_REG16) {
    SET("add ax, bx")
        T("01D8")

        SET("add bX, Ax")
        T("01C3")

        SET("add DX, AX")
        T("01C2")

        SET("add cx, dx")
        T("01D1")
}

TEST_F(AddFixture, MEM16_REG16) {
    SET("add [11], ax")
        T("01061100")

        SET("add [bx+si], cx")
        T("0108")

        SET("add [bx+si+fd], bx")
        T("0198FD00")

        SET("add [bp+si+dd], dx")
        T("0192DD00")

        SET("add wptr [bf], bx")
        T("011EBF00")
}

TEST_F(AddFixture, REG8_MEM8) {
    SET("add ah, bptr [bx+si]")
        T("0220")

        SET("add ah, wptr [bx+si]")
        T("")

        SET("add ch, [1f]")
        T("022E1F00")

        SET("add ch, bptr [1f]")
        T("022E1F00")

        SET("add dl, [bx+fb]")
        T("0297FB00")

        SET("add bl, [bx+si+cf]")
        T("0298CF00")

        SET("add al, [si+bp+2f]")
        T("02422F")
}

TEST_F(AddFixture, REG16_MEM16) {
    SET("add ax, wptr [11]")
        T("03061100")

        SET("add ax, [11]")
        T("03061100")

        SET("add ax, bptr [11]")
        T("")

        SET("add bx, [bx+si+ff]")
        T("0398FF00")

        SET("add cx, wptr [si+bp]")
        T("030A")

        SET("add dx, [si+bp+ffff]")
        T("0392FFFF")

        SET("add ax, [si+bp+ffff]")
        T("0382FFFF")

        SET("add cx, wptr [bx+si+fff]")
        T("0388FF0F");
}

TEST_F(AddFixture, REG8_IMMED8) {
    SET("add al, 05")
        T("0405")

        SET("add bl, 55")
        T("80C355")

        SET("add cl, ff")
        T("80C1FF")

        SET("add bh, 23")
        T("80C723")

        SET("add dh, 55")
        T("80C655")
}

TEST_F(AddFixture, MEM8_IMMED8) {
    SET("add bptr [ab], ff32")
        T("")

        SET("add bptr [ab], ff")
        T("8006AB00FF")

        SET("add bptr [ffff], 12")
        T("8006FFFF12")

        SET("add bptr [2d4b], 22")
        T("80064B2D22")

        SET("add bptr [bx+si+22], 44")
        T("80402244")

        SET("add bptr[bp+di],24")
        T("800324")

        SET("add bptr [bp+si+f24a], 21")
        T("80824AF221")

        SET("add [bp+si], 22")
        T("")
}

TEST_F(AddFixture, REG16_IMMED16) {
    SET("add ax, 543")
        T("054305")

        SET("add bx, 23ff")
        T("81C3FF23")

        SET("add cx, 2312")
        T("81C11223")

        SET("add dx, 1")
        T("81C20100")

        SET("add cs, 10")
        T("")
}

TEST_F(AddFixture, MEM16_IMMED16) {
    SET("add wptr [f1], 32")
        T("8106F1003200")

        SET("add wptr [bx+11], ffa2")
        T("814711A2FF")

        SET("add wptr [bp+si], ffff")
        T("8102FFFF")

        SET("add wptr [bp+di+f45b], 3fe1")
        T("81835BF4E13F")

        SET("add wptr [bx+si+3], 23df")
        T("814003DF23")

        SET("add wptr [bx+si+232], f1e")
        T("818032021E0F")
}


