#include "tree/binary_search_tree.hpp"
#include "set/set.hpp"
#include "models/student.hpp"

#include <cstddef>
#include <ios>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <random>

// Вспомогательные функции ввода данных
template <typename T>
T readInput(const std::string& prompt) {
    std::cout << prompt;
    T val;
    if (!(std::cin >> val)) {
        throw std::runtime_error("Invalid input");
    }
    return val;
}

template <>
Student readInput<Student>(const std::string& prompt) {
    std::cout << prompt << "\n";
    const auto id = readInput<uint64_t>("  ID: ");
    const auto first = readInput<std::string>("  First name: ");
    const auto last = readInput<std::string>("  Last name: ");
    const auto year = readInput<int>("  Birth year: ");
    const auto gpa = readInput<double>("  GPA: ");

    return Student(id, first, last, year, gpa);
}

// Генератор тестовых случайных данных
template <typename T>
void generateRandomData(BinarySearchTree<T>& tree, std::size_t count);

template <>
void generateRandomData<int>(BinarySearchTree<int>& tree, std::size_t count) {
    if (tree.size() >= count) return;

    if (count > static_cast<std::size_t>(std::numeric_limits<int>::max() / 5)) {
        throw std::invalid_argument("Requested count is too large");
    }

    std::mt19937 rng(100);
    std::uniform_int_distribution<int> dist(1, count * 5);

    while (tree.size() < count) {
        tree.insert(dist(rng));
    }
}

template <>
void generateRandomData<double>(BinarySearchTree<double>& tree, std::size_t count) {
    if (tree.size() >= count) return;

    constexpr std::size_t maxGeneratedValues = 991;
    if (count > maxGeneratedValues) {
        throw std::invalid_argument(
            "Automatic generation supports at most 991 double values"
        );
    }

    std::mt19937 rng(100);
    std::uniform_real_distribution<double> dist(1.0, 100.0);

    while (tree.size() < count) {
        tree.insert(std::round(dist(rng) * 10.0) / 10.0);
    }
}

template <>
void generateRandomData<std::string>(BinarySearchTree<std::string>& tree, std::size_t count) {
    static const char* words[] = {"Apple", "Banana", "Orange", "Cherry", "Mango", "Peach", "Plum", "Kiwi", "Lemon", "Melon"};

    for (std::size_t i = 0; i < count && i < 10; ++i) {
        tree.insert(words[i]);
    }
}

template <>
void generateRandomData<Student>(BinarySearchTree<Student>& tree, std::size_t count) {
    static const char* names[] = {"Ivan", "Petr", "Anna", "Elena", "Dmitry", "Sergey", "Olga"};
    static const char* lasts[] = {"Ivanov", "Petrov", "Sidorov", "Smirnov", "Kozlov", "Morozov", "Popov"};

    for (std::size_t i = 1; i <= count && i <= 7; ++i) {
        tree.insert(Student(100 + i, names[i - 1], lasts[i - 1], 2002 + (i % 3), 3.5 + (i % 15) * 0.1));
    }
}

template <typename T>
void runTreeMenu(const std::string& typeName) {
    BinarySearchTree<T> tree;

    while (true) {
        std::cout << "\n======================================================\n";
        std::cout << "   MENU: Type [" << typeName << "] (Current Size: " << tree.size() << ")\n";
        std::cout << "======================================================\n";
        std::cout << " 1. Insert element\n";
        std::cout << " 2. Remove element\n";
        std::cout << " 3. Search element (search / contains)\n";
        std::cout << " 4. Print tree (ASCII visualization)\n";
        std::cout << " 5. Traverse tree (KLP, KPL, LKP, PKL, LPK, PLK, BFS)\n";
        std::cout << " 6. Extract subtree by root value\n";
        std::cout << " 7. Serialize / Deserialize string round-trip\n";
        std::cout << " 8. Auto-generate random test elements\n";
        std::cout << " 9. Demonstrate Set operations (Set: +, *, -, <=)\n";
        std::cout << " 0. Return to main menu\n";
        std::cout << "Select option: ";

        int choice;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );
            std::cout << "Invalid menu option\n";
            continue;
        }

        if (choice == 0) break;

        try {
            switch (choice) {
                case 1: {
                    T val = readInput<T>("Enter value to insert: ");
                    if (tree.insert(val)) {
                        std::cout << "-> Element successfully inserted\n";
                    } else {
                        std::cout << "-> Element already exists in tree (duplicates prohibited)!\n";
                    }
                    break;
                }
                case 2: {
                    T val = readInput<T>("Enter value to remove: ");
                    if (tree.remove(val)) {
                        std::cout << "-> Element successfully removed\n";
                    } else {
                        std::cout << "-> Element not found in tree\n";
                    }
                    break;
                }
                case 3: {
                    T val = readInput<T>("Enter value to search: ");
                    if (tree.contains(val)) {
                        std::cout << "-> Element found: " << tree.search(val) << "\n";
                    } else {
                        std::cout << "-> Element does not exist in tree\n";
                    }
                    break;
                }
                case 4: {
                    std::cout << "\nTree structure:\n";
                    tree.printAscii();
                    break;
                }
                case 5: {
                    std::cout << "Select traversal order:\n";
                    std::cout << " 1: KLP (Pre-order:  Root, Left, Right)\n";
                    std::cout << " 2: KPL (Root, Right, Left)\n";
                    std::cout << " 3: LKP (In-order:   Left, Root, Right - Sorted)\n";
                    std::cout << " 4: PKL (Reverse In: Right, Root, Left)\n";
                    std::cout << " 5: LPK (Post-order: Left, Right, Root)\n";
                    std::cout << " 6: PLK (Right, Left, Root)\n";
                    std::cout << " 7: BFS (Breadth-First / Level-order)\n";
                    std::cout << "Order [1-7]: ";

                    int o = readInput<int>("");
                    TraversalOrder order = TraversalOrder::LKP;
                    if (o == 1) order = TraversalOrder::KLP;
                    else if (o == 2) order = TraversalOrder::KPL;
                    else if (o == 3) order = TraversalOrder::LKP;
                    else if (o == 4) order = TraversalOrder::PKL;
                    else if (o == 5) order = TraversalOrder::LPK;
                    else if (o == 6) order = TraversalOrder::PLK;
                    else if (o == 7) order = TraversalOrder::BFS;

                    auto items = tree.traverse(order);
                    std::cout << "Traversal result: [";
                    for (std::size_t i = 0; i < items.size(); ++i) {
                        std::cout << items[i] << (i + 1 < items.size() ? ", " : "");
                    }
                    std::cout << "]\n";
                    break;
                }
                case 6: {
                    T val = readInput<T>("Enter root value of subtree to extract: ");
                    auto sub = tree.getSubtree(val);
                    std::cout << "Extracted subtree:\n";
                    sub.printAscii();
                    break;
                }
                case 7: {
                    std::string s = tree.serialize();
                    std::cout << "Serialized string (KLP with '#'): " << s << "\n";
                    auto restored = BinarySearchTree<T>::deserialize(s);
                    std::cout << "Deserialized tree equality check: "
                              << (tree == restored ? "SUCCESS (Identical)" : "FAILURE (Mismatch)") << "\n";
                    break;
                }
                case 8: {
                    std::size_t count = readInput<std::size_t>("How many random elements to generate? ");
                    generateRandomData(tree, count);
                    std::cout << "-> Generated. Current size: " << tree.size() << "\n";
                    break;
                }
                case 9: {
                    std::cout << "\n--- Set Algebra Demonstration ---\n";
                    Set<T> setA;
                    Set<T> setB;
                    // Заполняем A элементами текущего дерева
                    tree.forEach(TraversalOrder::LKP, [&setA](const T& x) { setA.insert(x); });

                    std::cout << "Set A (populated from current tree): " << setA << "\n";
                    std::cout << "Enter 2 additional elements for Set B:\n";
                    setB.insert(readInput<T>("B[1]: "));
                    setB.insert(readInput<T>("B[2]: "));
                    if (!tree.empty()) {
                        // Добавим в B один общий элемент из A для наглядности пересечения
                        auto items = tree.traverse(TraversalOrder::LKP);
                        setB.insert(items[0]);
                    }
                    std::cout << "Set B: " << setB << "\n";

                    std::cout << "Union (A + B):  " << (setA + setB) << "\n";
                    std::cout << "Intersection (A * B):  " << (setA * setB) << "\n";
                    std::cout << "Difference (A - B):     " << (setA - setB) << "\n";
                    std::cout << "Subset check (B <= A):    " << (setB <= setA ? "True" : "False") << "\n";
                    std::cout << "Serialized Set A:   " << setA.serialize() << "\n";
                    break;
                }
                default:
                    std::cout << "Unknown menu option\n";
            }
        } catch (const std::exception& ex) {
            if (std::cin.eof()) break;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );
            }

            std::cout << "-> ERROR: " << ex.what() << "\n";
        }
    }
}

int main() {
    while (true) {
        std::cout << "\n======================================================\n";
        std::cout << "   BINARY SEARCH TREES AND SETS   \n";
        std::cout << "======================================================\n";
        std::cout << "Select data type for interactive testing:\n";
        std::cout << " 1. Integer numbers (int)\n";
        std::cout << " 2. Real numbers (double)\n";
        std::cout << " 3. Strings (std::string)\n";
        std::cout << " 4. User model (Student)\n";
        std::cout << " 0. Exit\n";
        std::cout << "Your choice: ";

        int choice;
        if (!(std::cin >> choice)) break;

        if (choice == 0) break;
        if (choice == 1) runTreeMenu<int>("int");
        else if (choice == 2) runTreeMenu<double>("double");
        else if (choice == 3) runTreeMenu<std::string>("std::string");
        else if (choice == 4) runTreeMenu<Student>("Student");
        else std::cout << "Invalid choice. Please try again\n";
    }

    std::cout << "Program finished\n";
    return 0;
}
