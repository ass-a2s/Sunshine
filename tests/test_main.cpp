/**
 * @file tests/test_main.cpp
 * @brief Test src/main.*.
 */

#include <gtest/gtest.h>
#include <src/main.h>

TEST(MainTestSuite, WriteFileTest) {
  EXPECT_EQ(write_file("write_file_test.txt", "test"), 0);
}

TEST(MainTestSuite, ReadFileTest) {
  // read file from WriteFileTest
  EXPECT_EQ(read_file("write_file_test.txt"), "test");

  // read missing file
  EXPECT_EQ(read_file("non-existing-file.txt"), "");
}
