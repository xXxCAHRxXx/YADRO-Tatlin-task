#pragma once

#include <filesystem>
#include <string>

#include "ITapeFactory.hpp"
#include "config/FileConfig.hpp"
#include "config/TapeDelays.hpp"
#include "tape/DelayedTape.hpp"
#include "tape/FileTape.hpp"

namespace fs = std::filesystem;

class DelayedFileTapeFactory : public ITapeFactory {
public:
    DelayedFileTapeFactory(const FileConfig& files, const TapeDelays& delays);

    std::unique_ptr<ITape> CreateInput() override;

    std::unique_ptr<ITape> CreateOutput() override;

    std::unique_ptr<ITape> CreateTemp() override;

    ~DelayedFileTapeFactory() override = default;

private:
    std::unique_ptr<ITape> CreateTape(const fs::path& path);

    FileConfig files_;
    TapeDelays delays_;
    std::size_t counter_ = 0;
};