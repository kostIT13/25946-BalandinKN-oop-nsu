#include <gtest/gtest.h>
#include <iostream>
#include "average.h"

TEST(AverageIntTest, PositiveNumbers) {
    EXPECT_EQ(ordinary::average<int>(10, 20), 15);
}

TEST(AverageIntTest, IntegerDivisionTruncates) {
    EXPECT_EQ(ordinary::average<int>(3, 4), 3);
}

TEST(AverageIntTest, NegativeNumbers) {
    EXPECT_EQ(ordinary::average<int>(-10, -20), -15);
}

TEST(AverageDoubleTest, KeepsFraction) {
    EXPECT_DOUBLE_EQ(ordinary::average<double>(3.0, 4.0), 3.5);
}

TEST(AverageDoubleTest, NegativeValues) {
    EXPECT_DOUBLE_EQ(ordinary::average<double>(-3.0, 4.0), 0.5);
}

TEST(AverageLongTest, BigValues) {
    EXPECT_EQ(ordinary::average<long long>(1'000'000'000LL, 3'000'000'000LL),
              2'000'000'000LL);
}

// Разница EXPECT / ASSERT видна только здесь: каждый макрос падает намеренно,
// маркеры после них показывают, докуда дошло тело теста.
TEST(AssertVsExpect, ExpectKeepsGoing) {
    EXPECT_EQ(ordinary::average<int>(5, 5), 99) << "EXPECT check #1";
    std::cout << ">>> тело живёт после падения #1\n";
    EXPECT_EQ(ordinary::average<int>(7, 7), 99) << "EXPECT check #2";
    std::cout << ">>> тело живёт после падения #2\n";
    EXPECT_EQ(ordinary::average<int>(9, 9), 99) << "EXPECT check #3";
    std::cout << ">>> дошли до конца теста\n";
}

TEST(AssertVsExpect, AssertStopsImmediately) {
    ASSERT_EQ(ordinary::average<int>(5, 5), 99) << "ASSERT check #1";
    std::cout << ">>> ЭТА СТРОКА НЕ ВЫПОЛНИТСЯ\n";
    ADD_FAILURE() << "ЭТА ПРОВЕРКА Тоже НЕ ВЫПОЛНИТСЯ";
}
