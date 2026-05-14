#pragma once

#include <stdexcept>

class TapeException : public std::runtime_error {
public:
    TapeException(const std::string& msg);
};