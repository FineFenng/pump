//
// Created by finefenng on 2018/10/13.
//

#include <pump/Singleton.hpp>
#include <gtest/gtest.h>

int add(int a, int b) {
  return a + b;
}

TEST(ADD, EQ) {
  EXPECT_EQ(3, add(1, 2));
}

GTEST_API_ int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}