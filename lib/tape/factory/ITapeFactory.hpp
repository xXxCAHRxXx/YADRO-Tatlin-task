#pragma once

#include <memory>

#include "../ITape.hpp"

class ITapeFactory {
public:
    virtual std::unique_ptr<ITape> CreateInput() = 0;

    virtual std::unique_ptr<ITape> CreateOutput() = 0;

    virtual std::unique_ptr<ITape> CreateTemp() = 0;

    virtual ~ITapeFactory() = default;
};