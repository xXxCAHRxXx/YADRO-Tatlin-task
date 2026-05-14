#pragma once

#include <chrono>

struct TapeDelays {
    std::chrono::milliseconds read{0};
    std::chrono::milliseconds write{0};
    std::chrono::milliseconds move{0};
    std::chrono::milliseconds rewind{0};
};