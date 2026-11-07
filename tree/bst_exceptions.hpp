#pragma once

#include <stdexcept>
#include <string>

class NodeNotFoundException : public std::runtime_error {
public:
    explicit NodeNotFoundException(const std::string& msg)
        : std::runtime_error("NodeNotFoundException: " + msg) {}
};

class EmptyTreeException : public std::runtime_error {
public:
    explicit EmptyTreeException(const std::string& msg = "Tree is empty")
        : std::runtime_error("EmptyTreeException: " + msg) {}
};

class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& msg)
        : std::runtime_error("ParseException: " + msg) {}
};
