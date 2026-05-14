#include <iostream>

#include <fstream>
#include <filesystem>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include "lib/config/FileConfig.hpp"
#include "lib/config/TapeDelays.hpp"
#include "lib/exception/TapeException.hpp"
#include "lib/tape/factory/DelayedFileTapeFactory.hpp"
#include "lib/sorter/TapeSorter.hpp"

namespace fs = std::filesystem;

struct Config {
    TapeDelays delays;
    std::size_t memory_limit;
};

Config LoadConfig(const fs::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Error: cannot open config file: " + path.string());
    }

    nlohmann::json json;
    file >> json;

    return Config{
        .delays = TapeDelays{
            .read   = std::chrono::milliseconds(json["delays"]["read_ms"]),
            .write  = std::chrono::milliseconds(json["delays"]["write_ms"]),
            .move   = std::chrono::milliseconds(json["delays"]["move_ms"]),
            .rewind = std::chrono::milliseconds(json["delays"]["rewind_ms"])
        },
        .memory_limit = json["memory"]["limit_bytes"]
    };
}

int main(int argc, char* argv[]) {
    CLI::App app("Tape sorter - sorts data from input tape to output tape.");

    std::string input;
    std::string output;
    std::string config_path;
    std::string tmp_dir = (std::filesystem::current_path() / "tmp").string();

    app.add_option("-i,--input", input, "Input tape file")->required()->check(CLI::ExistingFile);
    app.add_option("-o,--output", output, "Output file or directory")->required();
    app.add_option("-c,--config", config_path, "Config file")->required()->check(CLI::ExistingFile);
    app.add_option("-t,--tmp", tmp_dir, "Temporary directory");

    CLI11_PARSE(app, argc, argv);

    try {
        auto config = LoadConfig(config_path);

        FileConfig files{
            .input   = input,
            .output  = output,
            .tmp_dir = tmp_dir
        };

        auto factory = std::make_unique<DelayedFileTapeFactory>(files, config.delays);
        TapeSorter sorter(std::move(factory), config.memory_limit);
        sorter.Sort();
    } catch (const TapeException& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}