#include "TapeException.hpp"

TapeException::TapeException(const std::string& msg)
: std::runtime_error(msg)
{ }