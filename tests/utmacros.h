#ifndef UTMACROS_H
#define UTMACROS_H

#include <assembler/include/base.h>
#include <gtest/gtest.h>

#define T(x) EXPECT_STRCASEEQ(std::get<0>(b->process()).toStdString().c_str(), x)
#define SET(x) b->setCodeLine(x)

#endif
