#include "DelayedFileTapeFactory.hpp"

DelayedFileTapeFactory::DelayedFileTapeFactory(const FileConfig& files, const TapeDelays& delays)
: files_(files)
, delays_(delays) 
{
    if (!fs::is_regular_file(files_.input)) {
        throw TapeException("Error: input file does not exist: " + files_.input.string() + ".");
    }

    if (fs::exists(files_.tmp_dir) && !fs::is_directory(files_.tmp_dir)) {
        throw TapeException("Error: tmp_dir is not a directory: " + files_.tmp_dir.string() + ".");
    }

    fs::create_directories(files_.tmp_dir);
}

std::unique_ptr<ITape> DelayedFileTapeFactory::CreateInput() {
    return CreateTape(files_.input);
}
    
std::unique_ptr<ITape> DelayedFileTapeFactory::CreateOutput() {
    fs::path path;
    if (fs::is_directory(files_.output)) {
        path = files_.output / files_.input.filename();
    } else {
        path = files_.output;
    }

    if (fs::exists(path)) {
        std::ofstream(path, std::ios::binary | std::ios::trunc);
    }    

    return CreateTape(path);
}

std::unique_ptr<ITape> DelayedFileTapeFactory::CreateTemp() {
    auto path = files_.tmp_dir / (std::to_string(counter_++) + ".tmp");

    if (fs::exists(path)) {
        std::ofstream(path, std::ios::binary | std::ios::trunc);
    }

    return CreateTape(path);
}


std::unique_ptr<ITape> DelayedFileTapeFactory::CreateTape(const fs::path& path) {
    return std::make_unique<DelayedTape>(std::make_unique<FileTape>(path), delays_);
}