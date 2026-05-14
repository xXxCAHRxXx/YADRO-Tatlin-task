#pragma once

#include <vector>

#include "InMemoryTape.hpp"
#include "lib/tape/factory/ITapeFactory.hpp"

class InMemoryTapeFactory : public ITapeFactory {
public:
    InMemoryTapeFactory(const std::vector<int32_t>& input) 
    : input_(input)
    { }

    std::unique_ptr<ITape> CreateInput() override {
        return std::make_unique<InMemoryTape>(input_);
    }

    std::unique_ptr<ITape> CreateOutput() override {
        return std::make_unique<InMemoryTape>();
    }

    std::unique_ptr<ITape> CreateTemp() override {
        return std::make_unique<InMemoryTape>();
    }
private:
    std::vector<int32_t> input_;
};