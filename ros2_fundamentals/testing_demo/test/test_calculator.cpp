#include <gtest/gtest.h>
#include "testing_demo/calculator.hpp"

TEST(CalculatorTest, Add){
  Calculator calculator;

  EXPECT_EQ(
    calculator.add(2, 3),
    5);
}

TEST(CalculatorTest, Multiply){
  Calculator calculator;

  EXPECT_EQ(
    calculator.multiply(4, 5),
    20);
}

TEST(CalculatorTest, NegativeNumbers){
  Calculator calculator;

  EXPECT_EQ(
    calculator.add(-2, 3),
    1);
}