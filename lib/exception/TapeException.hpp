#pragma once

#include <stdexcept>
#include <string>

class TapeException : public std::runtime_error {
public:
    TapeException(const std::string& msg);
};