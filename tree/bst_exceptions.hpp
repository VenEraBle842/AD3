#pragma once

#include <stdexcept>
#include <string>

class NodeNotFoundException : public std::runtime_error {
public:
    explicit NodeNotFoundException(const std::string& msg)
        : std::runtime_error("NodeNotFoundException: " + msg) {}
};

class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& msg)
        : std::runtime_error("ParseException: " + msg) {}
};
