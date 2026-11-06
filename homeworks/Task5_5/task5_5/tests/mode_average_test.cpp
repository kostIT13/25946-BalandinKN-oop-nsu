#include <gtest/gtest.h>
#include "mode_average.h"

TEST(ModeAverageIntTest, WithinBounds) {
    for (int i = 0; i < 100; ++i) {
        int r = modified::mode_average<int>(100, 100);
        EXPECT_GE(r, 90);
        EXPECT_LE(r, 110);
    }
}

TEST(ModeAverageIntTest, ZeroInput) {
    for (int i = 0; i < 50; ++i) {
        int r = modified::mode_average<int>(0, 0);
        EXPECT_GE(r, -10);
        EXPECT_LE(r, 10);
    }
}

TEST(ModeAverageDoubleTest, WithinBounds) {
    for (int i = 0; i < 100; ++i) {
        double r = modified::mode_average<double>(100.0, 100.0);
        EXPECT_GE(r, 90.0);
        EXPECT_LE(r, 110.0);
    }
}

TEST(ModeAverageDoubleTest, KeepsFraction) {
    bool hasFraction = false;
    for (int i = 0; i < 100; ++i) {
        double r = modified::mode_average<double>(1.0, 2.0);
        if (r != static_cast<int>(r)) { hasFraction = true; break; }
    }
    EXPECT_TRUE(hasFraction);
}