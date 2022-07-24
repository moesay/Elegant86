#include <gtest/gtest.h>
#include <assembler/include/precom.h>
#include <iostream>

#include "mov.hpp"
#include "and.hpp"
#include "add.hpp"
#include "push.hpp"
#include "pop.hpp"
#include "sub.hpp"

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
