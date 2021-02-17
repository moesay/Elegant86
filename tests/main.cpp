#include "mainwindow.h"
#include <QApplication>
#include <gtest/gtest.h>
#include <assembler/include/precom.h>
#include <iostream>
#include "mov.hpp"
#include "add.hpp"
#include "push.hpp"

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
