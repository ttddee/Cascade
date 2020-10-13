#ifndef TST_TESTTEST_H
#define TST_TESTTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(CascadeTests, testtest)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

#endif // TST_TESTTEST_H
