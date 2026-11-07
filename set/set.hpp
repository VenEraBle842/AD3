#pragma once

#include "../tree/binary_search_tree.hpp"
#include "../tree/bst_exceptions.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>

template <typename T, typename Compare = std::less<T>>
class Set {
private:
    BinarySearchTree<T, Compare> tree_;

public:
    Set() = default;

    Set(std::initializer_list<T> init) {
        for (const auto& item : init) {
            tree_.insert(item);
        }
    }

    ~Set() = default;
    Set(const Set& other) = default;
    Set& operator=(const Set& other) = default;
    Set(Set&& other) noexcept = default;
    Set& operator=(Set&& other) noexcept = default;

    std::size_t size() const noexcept { return tree_.size(); }
    bool empty() const noexcept { return tree_.empty(); }

    void clear() noexcept {
        tree_.clear();
    }

    bool insert(T value) {
        return tree_.insert(std::move(value));
    }

    bool remove(const T& key) {
        return tree_.remove(key);
    }

    bool contains(const T& key) const {
        return tree_.contains(key);
    }

    // Объединение (A + B)
    Set<T, Compare> unionWith(const Set& other) const {
        Set<T, Compare> result = *this;
        other.tree_.forEach(TraversalOrder::LKP, [&result](const T& val) {
            result.insert(val);
        });
        return result;
    }

    // Пересечение (A * B)
    Set<T, Compare> intersectWith(const Set& other) const {
        return where([&other](const T& value) {
            return other.contains(value);
        });
    }

    // Разность (A - B)
    Set<T, Compare> differenceWith(const Set& other) const {
        return where([&other](const T& value) {
            return !other.contains(value);
        });
    }

    // A <= B (является ли текущее множество подмножеством other)
    [[nodiscard]] bool isSubsetOf(const Set& other) const {
        if (size() > other.size()) return false;

        bool allContained = true;
        tree_.forEach(TraversalOrder::LKP, [&allContained, &other](const T& val) {
            if (!allContained) return;
            if (!other.contains(val)) {
                allContained = false;
            }
        });
        return allContained;
    }

    template <typename Func>
    auto map(Func f) const -> Set<decltype(f(std::declval<T>()))> {
        using MappedType = decltype(f(std::declval<T>()));
        Set<MappedType> resultSet;
        tree_.forEach(TraversalOrder::LKP, [&resultSet, &f](const T& val) {
            resultSet.insert(f(val));
        });
        return resultSet;
    }

    template <typename Predicate>
    Set<T, Compare> where(Predicate p) const {
        Set<T, Compare> resultSet;
        tree_.forEach(TraversalOrder::KLP, [&resultSet, &p](const T& val) {
            if (p(val)) {
                resultSet.insert(val);
            }
        });
        return resultSet;
    }

    template <typename ResultType, typename Func>
    ResultType reduce(Func f, ResultType initial) const {
        return tree_.reduce(f, initial);
    }

    // Сериализация множества в строку формата "{ val1 val2 val3 }"
    std::string serialize() const {
        std::ostringstream oss;
        oss << "{ ";
        tree_.forEach(TraversalOrder::LKP, [&oss](const T& val) {
            oss << val << " ";
        });
        oss << "}";
        return oss.str();
    }

    static Set<T, Compare> deserialize(const std::string& str) {
        std::istringstream iss(str);
        std::string token;
        if (!(iss >> token) || token != "{") {
            throw ParseException("Set deserialization error: expected '{' at start");
        }

        Set<T, Compare> resultSet;
        while (iss >> token) {
            if (token == "}") {
                return resultSet;
            }
            std::stringstream ss(token);
            T val;
            if (!(ss >> val)) {
                throw ParseException("Set deserialization error: invalid element token: " + token);
            }
            resultSet.insert(std::move(val));
        }

        throw ParseException("Set deserialization error: expected '}' at end");
    }

    Set<T, Compare> operator+(const Set& other) const { return unionWith(other); }
    Set<T, Compare> operator*(const Set& other) const { return intersectWith(other); }
    Set<T, Compare> operator-(const Set& other) const { return differenceWith(other); }

    bool operator<=(const Set& other) const { return isSubsetOf(other); }

    bool operator==(const Set& other) const {
        return size() == other.size() && isSubsetOf(other);
    }

    bool operator!=(const Set& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Set& set) {
        os << "{";
        bool first = true;
        set.tree_.forEach(TraversalOrder::LKP, [&os, &first](const T& val) {
            if (!first) os << ", ";
            os << val;
            first = false;
        });
        os << "}";
        return os;
    }

    // Доступ к внутреннему дереву (для ASCII-печати в UI)
    const BinarySearchTree<T, Compare>& getInternalTree() const noexcept {
        return tree_;
    }
};
