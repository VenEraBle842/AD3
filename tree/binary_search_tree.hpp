#pragma once

#include "tree_node.hpp"
#include "bst_exceptions.hpp"
#include "traversal_types.hpp"
#include "tree_printer.hpp"
#include "../containers/dynamic_array.hpp"
#include "../containers/stack.hpp"
#include "../containers/queue.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>
#include <memory>

template <typename T, typename Compare = std::less<T>>
class BinarySearchTree {
private:
    std::shared_ptr<TreeNode<T>> root_{nullptr};
    std::size_t size_{0};
    Compare comp_{Compare()};

    enum class Step { Root, Left, Right };

    static std::shared_ptr<TreeNode<T>> cloneTree(const std::shared_ptr<TreeNode<T>>& srcRoot) {
        if (!srcRoot) return nullptr;

        auto newRoot = std::make_shared<TreeNode<T>>(srcRoot->value);
        Queue<std::pair<std::shared_ptr<TreeNode<T>>, std::shared_ptr<TreeNode<T>>>> q;
        q.push({srcRoot, newRoot});

        while (!q.empty()) {
            auto currentPair = q.front();
            q.pop();

            auto src = currentPair.first;
            auto dst = currentPair.second;

            if (src->left) {
                dst->left = std::make_shared<TreeNode<T>>(src->left->value);
                q.push({src->left, dst->left});
            }
            if (src->right) {
                dst->right = std::make_shared<TreeNode<T>>(src->right->value);
                q.push({src->right, dst->right});
            }
        }
        return newRoot;
    }

    std::shared_ptr<TreeNode<T>> findNode(const T& key) const {
        auto curr = root_;

        while (curr) {
            if (comp_(key, curr->value)) {
                curr = curr->left;
            } else if (comp_(curr->value, key)) {
                curr = curr->right;
            } else {
                return curr;
            }
        }

        return nullptr;
    }

    static void getStepsByOrder(TraversalOrder order, Step steps[3]) {
        switch (order) {
            case TraversalOrder::KLP: steps[0] = Step::Root;  steps[1] = Step::Left;  steps[2] = Step::Right; break;
            case TraversalOrder::KPL: steps[0] = Step::Root;  steps[1] = Step::Right; steps[2] = Step::Left;  break;
            case TraversalOrder::LKP: steps[0] = Step::Left;  steps[1] = Step::Root;  steps[2] = Step::Right; break;
            case TraversalOrder::PKL: steps[0] = Step::Right; steps[1] = Step::Root;  steps[2] = Step::Left;  break;
            case TraversalOrder::LPK: steps[0] = Step::Left;  steps[1] = Step::Right; steps[2] = Step::Root;  break;
            case TraversalOrder::PLK: steps[0] = Step::Right; steps[1] = Step::Left;  steps[2] = Step::Root;  break;
            default:                  steps[0] = Step::Left;  steps[1] = Step::Root;  steps[2] = Step::Right; break;
        }
    }

    template <typename Action>
    void traverseUniversalDFS(const Step steps[3], Action action) const {
        if (!root_) return;

        struct Task {
            std::shared_ptr<TreeNode<T>> node;
            bool isAction; // true: вызвать action над узлом, false: раскрыть поддеревья
        };

        Stack<Task> st;
        st.push({root_, false});

        while (!st.empty()) {
            Task current = st.top();
            st.pop();

            if (!current.node) continue;

            if (current.isAction) {
                action(std::as_const(current.node->value));
            } else {
                // Помещаем шаги на стек в ОБРАТНОМ порядке, чтобы первый шаг выполнился первым
                for (int i = 2; i >= 0; --i) {
                    if (steps[i] == Step::Root) {
                        st.push({current.node, true});
                    } else if (steps[i] == Step::Left) {
                        if (current.node->left) st.push({current.node->left, false});
                    } else if (steps[i] == Step::Right) {
                        if (current.node->right) st.push({current.node->right, false});
                    }
                }
            }
        }
    }

    template <typename Action>
    void traverseBFS(Action action) const {
        if (!root_) return;
        Queue<std::shared_ptr<TreeNode<T>>> q;
        q.push(root_);

        while (!q.empty()) {
            auto curr = q.front();
            q.pop();
            action(std::as_const(curr->value));

            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
    }

public:
    BinarySearchTree() = default;

    ~BinarySearchTree() {
        if (!root_) return;
        Queue<std::shared_ptr<TreeNode<T>>> q;
        q.push(root_);

        while (!q.empty()) {
            auto curr = q.front();
            q.pop();

            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);

            curr->left.reset();
            curr->right.reset();
        }
        root_.reset();
        size_ = 0;
    }

    BinarySearchTree(const BinarySearchTree& other)
        : size_(other.size_), comp_(other.comp_) {
        root_ = cloneTree(other.root_);
    }

    // Copy-and-Swap: старое состояние уничтожается через деструктор временного объекта
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            BinarySearchTree temp(other);
            std::swap(root_, temp.root_);
            std::swap(size_, temp.size_);
            std::swap(comp_, temp.comp_);
        }
        return *this;
    }

    BinarySearchTree(BinarySearchTree&& other) noexcept
    : root_(std::move(other.root_)),
      size_(std::exchange(other.size_, 0)),
      comp_(std::move(other.comp_)) {}

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            BinarySearchTree temp(std::move(*this)); // текущие ресурсы уйдут в деструктор temp
            root_ = std::move(other.root_);
            size_ = other.size_;
            comp_ = std::move(other.comp_);
            other.size_ = 0;
        }
        return *this;
    }

    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void clear() noexcept {
        *this = BinarySearchTree();
    }

    bool insert(T value) {
        auto newNode = std::make_shared<TreeNode<T>>(std::move(value));
        if (!root_) {
            root_ = newNode;
            ++size_;
            return true;
        }

        auto curr = root_;
        while (true) {
            if (comp_(newNode->value, curr->value)) {
                if (!curr->left) {
                    curr->left = newNode;
                    ++size_;
                    return true;
                }
                curr = curr->left;
            } else if (comp_(curr->value, newNode->value)) {
                if (!curr->right) {
                    curr->right = newNode;
                    ++size_;
                    return true;
                }
                curr = curr->right;
            } else {
                return false; // Уникальные ключи
            }
        }
    }

    bool contains(const T& key) const {
        return findNode(key) != nullptr;
    }

    const T& search(const T& key) const {
        auto node = findNode(key);
        if (!node) throw NodeNotFoundException("Key not found in tree");
        return node->value;
    }

    bool remove(const T& key) {
        std::shared_ptr<TreeNode<T>> parent = nullptr;
        auto curr = root_;

        while (curr) {
            if (comp_(key, curr->value)) {
                parent = curr;
                curr = curr->left;
            } else if (comp_(curr->value, key)) {
                parent = curr;
                curr = curr->right;
            } else {
                break;
            }
        }

        if (!curr) return false;

        // Случай 1: у узла не более одного потомка
        if (!curr->left || !curr->right) {
            auto child = curr->left ? curr->left : curr->right;
            if (!parent) {
                root_ = child;
            } else if (parent->left == curr) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        } else {
            // Случай 2: у узла два потомка
            auto succParent = curr;
            auto succ = curr->right;
            while (succ->left) {
                succParent = succ;
                succ = succ->left;
            }

            curr->value = std::move(succ->value);
            if (succParent->left == succ) {
                succParent->left = succ->right;
            } else {
                succParent->right = succ->right;
            }
        }

        --size_;
        return true;
    }

    // Извлечение поддерева по значению корня
    BinarySearchTree<T, Compare> getSubtree(const T& rootValue) const {
        auto curr = findNode(rootValue);
        if (!curr) throw NodeNotFoundException("Subtree root value was not found in tree");

        BinarySearchTree<T, Compare> result;
        result.root_ = cloneTree(curr);

        std::size_t subCount = 0;
        result.forEach(TraversalOrder::LKP, [&subCount](const T&) { ++subCount; });
        result.size_ = subCount;

        return result;
    }

    template <typename Action>
    void forEach(TraversalOrder order, Action action) const {
        if (order == TraversalOrder::BFS) {
            traverseBFS(action);
        } else {
            Step steps[3];
            getStepsByOrder(order, steps);
            traverseUniversalDFS(steps, action);
        }
    }

    // Получение массива по TraversalOrder
    DynamicArray<T> traverse(TraversalOrder order) const {
        DynamicArray<T> result;
        forEach(order, [&result](const T& val) {
            result.push_back(val);
        });
        return result;
    }

    template <typename Func>
    auto map(Func f) const -> BinarySearchTree<decltype(f(std::declval<T>()))> {
        using MappedType = decltype(f(std::declval<T>()));
        BinarySearchTree<MappedType> newTree;
        forEach(TraversalOrder::KLP, [&newTree, &f](const T& val) {
            newTree.insert(f(val));
        });
        return newTree;
    }

    template <typename Predicate>
    BinarySearchTree<T, Compare> where(Predicate p) const {
        BinarySearchTree<T, Compare> filtered;
        forEach(TraversalOrder::KLP, [&filtered, &p](const T& val) {
            if (p(val)) {
                filtered.insert(val);
            }
        });
        return filtered;
    }

    template <typename ResultType, typename Func>
    ResultType reduce(Func f, ResultType initial) const {
        ResultType acc = initial;
        forEach(TraversalOrder::LKP, [&acc, &f](const T& val) {
            acc = f(acc, val);
        });
        return acc;
    }

    // Итеративная сериализация (Pre-order с маркером #)
    std::string serialize() const {
        if (!root_) return "#";
        std::ostringstream oss;
        Stack<std::shared_ptr<TreeNode<T>>> st;
        st.push(root_);

        while (!st.empty()) {
            auto curr = st.top();
            st.pop();

            if (!curr) {
                oss << "# ";
            } else {
                oss << curr->value << " ";
                st.push(curr->right);
                st.push(curr->left);
            }
        }
        return oss.str();
    }

    static BinarySearchTree<T, Compare> deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        if (!(iss >> token) || token == "#") {
            return BinarySearchTree<T, Compare>();
        }

        std::stringstream ss(token);
        T rootVal;
        if (!(ss >> rootVal)) throw ParseException("Failed to parse token: " + token);

        BinarySearchTree<T, Compare> tree;
        tree.root_ = std::make_shared<TreeNode<T>>(std::move(rootVal));
        tree.size_ = 1;

        struct NodeSlot {
            std::shared_ptr<TreeNode<T>> parent;
            bool isLeft;
        };

        Stack<NodeSlot> st;
        st.push({tree.root_, false}); // правый слот
        st.push({tree.root_, true});  // левый слот

        while (iss >> token && !st.empty()) {
            auto slot = st.top();
            st.pop();

            if (token != "#") {
                std::stringstream nodeSS(token);
                T val;
                if (!(nodeSS >> val)) throw ParseException("Failed to parse token: " + token);

                auto newNode = std::make_shared<TreeNode<T>>(std::move(val));
                if (slot.isLeft) {
                    slot.parent->left = newNode;
                } else {
                    slot.parent->right = newNode;
                }
                ++tree.size_;

                st.push({newNode, false});
                st.push({newNode, true});
            }
        }
        return tree;
    }

    void printAscii() const {
        TreePrinter<T>::print(root_);
    }

    bool operator==(const BinarySearchTree& other) const {
        if (size_ != other.size_) return false;

        Stack<std::shared_ptr<TreeNode<T>>> st1, st2;
        auto c1 = root_, c2 = other.root_;

        while ((c1 || !st1.empty()) && (c2 || !st2.empty())) {
            while (c1) { st1.push(c1); c1 = c1->left; }
            while (c2) { st2.push(c2); c2 = c2->left; }

            c1 = st1.top(); st1.pop();
            c2 = st2.top(); st2.pop();

            if (c1->value != c2->value) return false;

            c1 = c1->right;
            c2 = c2->right;
        }
        return st1.empty() && st2.empty() && !c1 && !c2;
    }

    bool operator!=(const BinarySearchTree& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const BinarySearchTree& tree) {
        os << "[";
        bool first = true;
        tree.forEach(TraversalOrder::LKP, [&os, &first](const T& val) {
            if (!first) os << ", ";
            os << val;
            first = false;
        });
        os << "]";
        return os;
    }
};
