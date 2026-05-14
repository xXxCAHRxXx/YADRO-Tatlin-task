#pragma once

#include <memory>
#include <chrono>
#include <thread>

#include "ITape.hpp"
#include "config/TapeDelays.hpp"


class DelayedTape : public ITape {
public:
    DelayedTape(std::unique_ptr<ITape> tape, const TapeDelays& delays);

    int32_t Read() override;

    void Write(int32_t value) override;

    void MoveNext() override;

    void MoveBack() override;

    void Rewind() override;

    bool IsEnd() const override;

    ~DelayedTape() override = default;
private:
    void Sleep(std::chrono::milliseconds time);

    std::unique_ptr<ITape> tape_;
    TapeDelays delays_; 
};