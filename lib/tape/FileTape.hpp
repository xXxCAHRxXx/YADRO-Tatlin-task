#pragma once

#include "ITape.hpp"
#include "exception/TapeException.hpp"


#include <filesystem>
#include <fstream>

class FileTape : public ITape {
public:
    FileTape(const std::filesystem::path& path);

    int32_t Read() override;

    void Write(int32_t value) override;

    void MoveNext() override;

    void MoveBack() override;

    void Rewind() override;

    bool IsEnd() const override;

    ~FileTape() override = default;
private:
    void MoveToPos();

    static constexpr std::size_t kElementSize = sizeof(int32_t);

    std::fstream file_;
    std::size_t pos_ = 0;
    std::size_t size_ = 0;
};