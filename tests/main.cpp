// Renames main() in main.cpp to make room for the unit test main().
#define main SunshineMain

#include "gtest/gtest.h"

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
