#include "tests/conftest.cpp"

class DocsTests : public DocsTestFixture, public ::testing::WithParamInterface<std::tuple<const char *, const char *>> {};

TEST_P(DocsTests, MakeDocs) {
  auto params = GetParam();
  std::string format = std::get<0>(params);
  std::string expected_filename = std::get<1>(params);

  std::filesystem::path expected_file = std::filesystem::current_path() / "build" / format / expected_filename;

  std::string command = "make " + format;
  int status = std::system(command.c_str());
  EXPECT_EQ(status, 0);

  EXPECT_TRUE(std::filesystem::exists(expected_file));
}

INSTANTIATE_TEST_SUITE_P(DocFormats, DocsTests, ::testing::Values(
    std::make_tuple("html", "index.html"),
    std::make_tuple("epub", "Sunshine.epub")
), CustomNameGenerator());

TEST_F(DocsPythonVenv, RstCheck) {
  std::string command = "rstcheck -r .";
  int status = std::system(command.c_str());
  EXPECT_EQ(status, 0);
}
