#pragma once

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>

template <typename T>
class Queue {
private:
    std::unique_ptr<T[]> data_{nullptr};
    std::size_t capacity_{0};
    std::size_t head_{0};
    std::size_t tail_{0};
    std::size_t size_{0};

    void reallocate(std::size_t newCapacity) {
        auto newData = std::make_unique<T[]>(newCapacity);
        for (std::size_t i = 0; i < size_; ++i) {
            newData[i] = std::move(data_[(head_ + i) % capacity_]);
        }
        data_ = std::move(newData);
        head_ = 0;
        tail_ = size_;
        capacity_ = newCapacity;
    }

public:
    Queue() = default;

    ~Queue() = default;

    Queue(const Queue& other)
        : capacity_(other.capacity_), head_(0), tail_(other.capacity_ == 0 ? 0 : other.size_ % other.capacity_), size_(other.size_) {
        if (capacity_ > 0) {
            data_ = std::make_unique<T[]>(capacity_);
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[(other.head_ + i) % other.capacity_];
            }
        }
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            Queue copy(other);
            *this = std::move(copy);
        }
        return *this;
    }

    Queue(Queue&& other) noexcept
    : data_(std::move(other.data_)),
      capacity_(std::exchange(other.capacity_, 0)),
      head_(std::exchange(other.head_, 0)),
      tail_(std::exchange(other.tail_, 0)),
      size_(std::exchange(other.size_, 0)) {}

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            capacity_ = std::exchange(other.capacity_, 0);
            head_ = std::exchange(other.head_, 0);
            tail_ = std::exchange(other.tail_, 0);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    void push(T value) {
        if (size_ == capacity_) {
            reallocate(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[tail_] = std::move(value);
        tail_ = (tail_ + 1) % capacity_;
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("Queue underflow");
        head_ = (head_ + 1) % capacity_;
        --size_;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Queue is empty");
        return data_[head_];
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Queue is empty");
        return data_[head_];
    }

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

    void clear() noexcept {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }
};
