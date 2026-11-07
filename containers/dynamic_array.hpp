#pragma once

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <initializer_list>
#include <memory>

template <typename T>
class DynamicArray {
private:
    std::unique_ptr<T[]> data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};

    void reallocate(std::size_t newCapacity) {
        auto newData = std::make_unique<T[]>(newCapacity);
        for (std::size_t i = 0; i < size_; ++i) {
            newData[i] = std::move(data_[i]);
        }
        data_ = std::move(newData);
        capacity_ = newCapacity;
    }

public:
    DynamicArray() = default;

    explicit DynamicArray(std::size_t initialCapacity) {
        if (initialCapacity > 0) {
            capacity_ = initialCapacity;
            data_ = std::make_unique<T[]>(capacity_);
        }
    }

    DynamicArray(std::initializer_list<T> init)
        : size_(init.size()), capacity_(init.size()) {
        if (capacity_ > 0) {
            data_ = std::make_unique<T[]>(capacity_);
            std::size_t i = 0;
            for (const auto& item : init) {
                data_[i++] = item;
            }
        }
    }

    ~DynamicArray() = default;

    DynamicArray(const DynamicArray& other)
        : size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            data_ = std::make_unique<T[]>(capacity_);
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            DynamicArray copy(other);
            *this = std::move(copy);
        }
        return *this;
    }

    DynamicArray(DynamicArray&& other) noexcept
    : data_(std::move(other.data_)),
      size_(std::exchange(other.size_, 0)),
      capacity_(std::exchange(other.capacity_, 0)) {}

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }

    void push_back(T value) {
        if (size_ == capacity_) {
            reallocate(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    T& operator[](std::size_t index) { return data_[index]; }
    const T& operator[](std::size_t index) const { return data_[index]; }

    T& at(std::size_t index) {
        if (index >= size_) throw std::out_of_range("DynamicArray: Index out of range");
        return data_[index];
    }

    const T& at(std::size_t index) const {
        if (index >= size_) throw std::out_of_range("DynamicArray: Index out of range");
        return data_[index];
    }

    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    void clear() noexcept {
        size_ = 0;
    }

    class Iterator {
    private:
        DynamicArray<T>& arr_;
        std::size_t index_;
    public:
        Iterator(DynamicArray<T>& arr, std::size_t idx) : arr_(arr), index_(idx) {}
        T& operator*() { return arr_[index_]; }
        Iterator& operator++() { ++index_; return *this; }
        bool operator!=(const Iterator& other) const { return index_ != other.index_; }
    };

    class ConstIterator {
    private:
        const DynamicArray<T>& arr_;
        std::size_t index_;
    public:
        ConstIterator(const DynamicArray<T>& arr, std::size_t idx) : arr_(arr), index_(idx) {}
        const T& operator*() const { return arr_[index_]; }
        ConstIterator& operator++() { ++index_; return *this; }
        bool operator!=(const ConstIterator& other) const { return index_ != other.index_; }
    };

    Iterator begin() { return Iterator(*this, 0); }
    Iterator end() { return Iterator(*this, size_); }
    ConstIterator begin() const { return ConstIterator(*this, 0); }
    ConstIterator end() const { return ConstIterator(*this, size_); }
};
