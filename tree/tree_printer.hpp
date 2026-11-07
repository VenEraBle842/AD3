#pragma once

#include "tree_node.hpp"
#include "../containers/stack.hpp"
#include <iostream>
#include <string>
#include <memory>

template <typename T>
class TreePrinter {
private:
    struct Frame {
        std::shared_ptr<TreeNode<T>> node;
        std::string prefix;
        bool isLeft;
        bool isRoot;
    };

public:
    static void print(const std::shared_ptr<TreeNode<T>>& root) {
        if (!root) {
            std::cout << "(empty tree)\n";
            return;
        }

        Stack<Frame> st;
        st.push({root, "", false, true});

        while (!st.empty()) {
            Frame curr = st.top();
            st.pop();

            std::cout << curr.prefix;
            if (curr.isRoot) {
                std::cout << "[*] ";
            } else {
                std::cout << (curr.isLeft ? "|-- [L] " : "+-- [R] ");
            }

            if (!curr.node) {
                std::cout << "#\n";
                continue;
            }

            std::cout << curr.node->value << "\n";

            std::string nextPrefix = curr.prefix + (curr.isRoot ? "" : (curr.isLeft ? "|   " : "    "));

            if (curr.node->left || curr.node->right) {
                st.push({curr.node->right, nextPrefix, false, false});
                st.push({curr.node->left, nextPrefix, true, false});
            }
        }
    }
};
