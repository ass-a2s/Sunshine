/**
 * @file main.cpp
 * @brief Main file for running tests
 */

#include "gtest/gtest.h"

/**
 * @brief Main entry point for running tests
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return 0 on success, 1 on failure
 */
int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
