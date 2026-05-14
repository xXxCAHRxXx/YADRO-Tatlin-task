#pragma once

#include <vector>

#include "lib/tape/ITape.hpp" 
#include "lib/exception/TapeException.hpp" 

class InMemoryTape : public ITape {
public:
    InMemoryTape() = default;

    InMemoryTape(const std::vector<int32_t>& data)
    : data_(data)
    { }

    int32_t Read() override {
        if (IsEnd()) {
            throw TapeException("Error: read at end.");
        }

        return data_[pos_];
    }

    void Write(int32_t value) override {
        if (IsEnd()) {
            data_.resize(pos_ + 1);
        }

        data_[pos_] = value;
    }

    void MoveNext() override {
        if (IsEnd()) {
            throw TapeException("Error: MoveNext at end.");
        }

        ++pos_;
    }

    void MoveBack() override {
        if (pos_ == 0) {
            throw TapeException("MoveBack at beginning");
        }

        --pos_;
    }

    void Rewind() override {
        pos_ = 0;
    }

    bool IsEnd() const override {
        return pos_ >= data_.size();
    }

    const std::vector<int32_t>& Data() const {
        return data_;
    }
private:
    std::vector<int32_t> data_;
    std::size_t pos_ = 0;
};
