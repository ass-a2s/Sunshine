/**
 * @file tests/test_file_handler.cpp
 * @brief Test src/file_handler.*.
 */

#include <gtest/gtest.h>
#include <src/file_handler.h>

TEST(FileHandlerTestSuite, WriteFileTest) {
  EXPECT_EQ(file_handler::write_file("write_file_test.txt", "test"), 0);
}

TEST(FileHandlerTestSuite, ReadFileTest) {
  // read file from WriteFileTest
  EXPECT_EQ(file_handler::read_file("write_file_test.txt"), "test");

  // read missing file
  EXPECT_EQ(file_handler::read_file("non-existing-file.txt"), "");
}
