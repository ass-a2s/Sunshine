#include <gtest/gtest.h>
#include <filesystem>

class CustomNameGenerator {
public:
  template<typename... Args>
  std::string operator()(const ::testing::TestParamInfo<std::tuple<Args...>>& info) const {
    std::string result = concatenate(info.param, std::index_sequence_for<Args...>{});
    std::replace(result.begin(), result.end(), '.', '_');
    return result;
  }

private:
  template<typename... Args, std::size_t... Is>
  std::string concatenate(const std::tuple<Args...>& tuple, std::index_sequence<Is...>) const {
    std::string result;
    ((result += (Is == 0 ? "" : "_") + std::string(std::get<Is>(tuple))), ...);
    return result;
  }
};

class DocsPythonVenvBase {
public:
  std::filesystem::path fullPythonPath;
  std::filesystem::path fullPythonBinPath;
  std::string originalEnvPath;

  void
  SetUp() {
    std::string pythonCommandArray[] = {"python3", "python"};
    std::string python;
    bool python_found = false;
    int exit_code;

    // loop over the commandArray to find the first command that works
    for (const std::string& command : pythonCommandArray) {
      if (std::system((command + " --version").c_str()) == 0) {
        python_found = true;
        python = command;
        break;
      }
    }

    // fail if python is not installed
    if (!python_found) {
      FAIL() << "Python is not installed";
    }

    // command array
    std::filesystem::path venvPath = ".venv";
    std::filesystem::path binPath = "bin";
    std::filesystem::path pythonPath = python;
    fullPythonPath = std::filesystem::current_path() / venvPath / binPath / pythonPath;
    fullPythonBinPath = std::filesystem::current_path() / venvPath / binPath;

    std::string CommandArray[] = {
      python + " -m venv .venv",
      fullPythonPath.string() + " -m pip install -r ./repo/docs/requirements.txt",
    };

    for (const std::string& command : CommandArray) {
      exit_code = std::system(command.c_str());
      if (exit_code != 0) {
        FAIL() << "Command failed: " << command;
      }
    }

    // Save the original PATH
    originalEnvPath = std::getenv("PATH") ? std::getenv("PATH") : "";

    // Set the temporary PATH
    std::string tempPath;
#ifdef _WIN32
    tempPath = fullPythonBinPath.string() + ";" + originalEnvPath;
#else
    tempPath = fullPythonBinPath.string() + ":" + originalEnvPath;
#endif
    putenv(("PATH=" + tempPath).c_str());
  }

  void
  TearDown() {
    // Restore the original PATH
    putenv(("PATH=" + originalEnvPath).c_str());
  }
};

class DocsWorkingDirectoryBase {
public:
  std::filesystem::path original_dir;
  std::filesystem::path temp_dir;

  void SetUp() {
    // get current directory
    original_dir = std::filesystem::current_path();

    temp_dir = original_dir / "repo" / "docs";

    // print temp_dir
    std::cout << "temp_dir: " << temp_dir << std::endl;

    // change directory to `docs`
    std::filesystem::current_path(temp_dir);
  }

  void TearDown() {
    // remove `repo/docs/build` directory
//    std::filesystem::path build_dir = temp_dir / "build";
//    if (std::filesystem::exists(build_dir)) {
//      std::filesystem::remove_all(build_dir);
//    }

    // change directory back to original
    std::filesystem::current_path(original_dir);
  }
};

class DocsPythonVenv : public ::testing::Test {
protected:
  DocsPythonVenvBase pythonVenv;

  void SetUp() override {
    pythonVenv.SetUp();
  }

  void TearDown() override {
    pythonVenv.TearDown();
  }
};

class DocsTestFixture : public ::testing::Test {
protected:
  DocsPythonVenvBase pythonVenv;
  DocsWorkingDirectoryBase workingDirectory;

  void SetUp() override {
    pythonVenv.SetUp();
    workingDirectory.SetUp();
  }

  void TearDown() override {
    pythonVenv.TearDown();
    workingDirectory.TearDown();
  }
};
