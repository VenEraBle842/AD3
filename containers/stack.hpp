#pragma once

#include "dynamic_array.hpp"
#include <stdexcept>
#include <utility>

template <typename T>
class Stack {
private:
    DynamicArray<T> buffer_;

public:
    Stack() = default;

    void push(T value) {
        buffer_.push_back(std::move(value));
    }

    void pop() {
        if (empty()) throw std::underflow_error("Stack underflow");
        buffer_.pop_back();
    }

    T& top() {
        if (empty()) throw std::underflow_error("Stack is empty");
        return buffer_[buffer_.size() - 1];
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("Stack is empty");
        return buffer_[buffer_.size() - 1];
    }

    bool empty() const noexcept { return buffer_.empty(); }
    std::size_t size() const noexcept { return buffer_.size(); }
    void clear() noexcept { buffer_.clear(); }
};
