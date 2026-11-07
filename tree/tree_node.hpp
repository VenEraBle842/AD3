#pragma once

#include <memory>
#include <utility>

template <typename T>
struct TreeNode {
    T value;
    std::shared_ptr<TreeNode<T>> left{nullptr};
    std::shared_ptr<TreeNode<T>> right{nullptr};

    explicit TreeNode(T val) : value(std::move(val)) {}
};
