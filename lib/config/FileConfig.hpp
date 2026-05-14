#pragma once

#include <filesystem>

namespace fs = std::filesystem;

struct FileConfig {
    fs::path input;
    fs::path output;
    fs::path tmp_dir;
};