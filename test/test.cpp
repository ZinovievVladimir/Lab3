#include <gtest/gtest.h>
#include "ParsePolish.h"

TEST(Parse, Example0) {
  Polish a("||-3^2+|-5|^3+|-5^3||-40|");

  double res = a.Resultof();

  EXPECT_EQ(201, res);
}

TEST(Parse, Example1) {
  Polish a("12-12-11/(3*3+2)-2^2");

  double res = a.Resultof();

  EXPECT_EQ(-5, res);
}

TEST(Parse, Throws1) {
  Polish a("(31+8a) - 7*3 +8 -14/(8-1)");

  ASSERT_ANY_THROW(a.Resultof());
}

TEST(Parse, Example2) {
  Polish a("(31+8) - 7*3 +8 -14/(8-1)");

  double res = a.Resultof();

  EXPECT_EQ(24, res);
}

TEST(Parse, Example3) {
  Polish a("|1 - 8| * 3");

  double res = a.Resultof();

  EXPECT_EQ(21, res);
}

TEST(Parse, Example4) {
  Polish a("|1 - a| * 3");

  a.AddVar('a', 8);
  double res = a.Resultof();

  EXPECT_EQ(21, res);
}


TEST(Parse, Example5) {
  Polish a("(a+8) - b*3 +8 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);
  a.AddVar('c', 14);

  double res = a.Resultof();

  EXPECT_EQ(24, res);
}

TEST(Parse, Example6) {
  Polish a("(a+8) + (-b)*3 +8 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);
  a.AddVar('c', 14);

  double res = a.Resultof();

  EXPECT_EQ(24, res);
}

TEST(Parse, Example7) {
  Polish a("-(a+8) - b*3 +8 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);
  a.AddVar('c', 14);

  double res = a.Resultof();

  EXPECT_EQ(-54, res);
}

TEST(Parse, Example8) {
  Polish a("(-(a+8)) - b*3 +8.3 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);
  a.AddVar('c', 14);

  double res = a.Resultof();

  EXPECT_EQ(-53.7, res);
}

TEST(Parse, Throws2) {
  Polish a("+-(a+8) - b*3 +8.3 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);
  a.AddVar('c', 14);

  ASSERT_ANY_THROW (a.Resultof());
}

TEST(Parse, NotDeclareVar) {
  Polish a("(a+8) - b*3 +8.3 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);

  ASSERT_ANY_THROW(a.Resultof());
}

TEST(Parse, Example11) {
  Polish a("(-(a+8)) - b*3 +8.3 -c/(8-1)");

  a.AddVar('a', 31);
  a.AddVar('b', 7);
  a.AddVar('c', 14);

  double res = a.Resultof();

  EXPECT_EQ(-53.7, res);
}

TEST(Parse, EmptyString) {
  ASSERT_ANY_THROW(Polish a(""));
}

TEST(Parse, Example12) {
  Polish a("3+5-|8-10*2+6/3|");

  double res = a.Resultof();

  EXPECT_EQ(-2, res);
}

TEST(Parse, Example13) {
  Polish a("||-3|+|-3||");

  double res = a.Resultof();

  EXPECT_EQ(6, res);
}

TEST(Parse, MoreOpenBrackets) {
  ASSERT_ANY_THROW(Polish a("|(|("));
}

TEST(Parse, MoreBrackets) {
  ASSERT_ANY_THROW(Polish a("|(|)"));
}

TEST(Parse, MoreBrackets2) {
  ASSERT_ANY_THROW(Polish a("()"));
}

TEST(Parse, MoreBrackets3) {
  ASSERT_ANY_THROW(Polish a("||"));
}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}