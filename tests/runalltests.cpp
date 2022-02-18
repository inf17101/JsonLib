#include <gtest/gtest.h>
#include "TestParser.cpp"
#include "TestLexer.cpp"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}