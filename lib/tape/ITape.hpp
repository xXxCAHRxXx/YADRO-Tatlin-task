#pragma once

#include <cstdint>

class ITape {
public:
    virtual int32_t Read() = 0;

    virtual void Write(int32_t value) = 0;

    virtual void MoveNext() = 0;

    virtual void MoveBack() = 0;

    virtual void Rewind() = 0;

    virtual bool IsEnd() const = 0;

    virtual ~ITape() = default;
};