#include <gtest/gtest.h>

#include "../tree/binary_search_tree.hpp"
#include "../tree/bst_exceptions.hpp"
#include "../models/student.hpp"

TEST(BSTTest, InsertSearchRemoveContains) {
    BinarySearchTree<int> bst;

    EXPECT_TRUE(bst.insert(50));
    EXPECT_TRUE(bst.insert(30));
    EXPECT_TRUE(bst.insert(70));
    EXPECT_TRUE(bst.insert(20));
    EXPECT_TRUE(bst.insert(40));
    EXPECT_FALSE(bst.insert(30)); // Дубликат запрещен

    EXPECT_EQ(bst.size(), 5);
    EXPECT_TRUE(bst.contains(50));
    EXPECT_TRUE(bst.contains(20));
    EXPECT_FALSE(bst.contains(999));

    EXPECT_EQ(bst.search(40), 40);
    EXPECT_THROW(bst.search(12345), NodeNotFoundException);

    // 1. Удаление листа (20)
    EXPECT_TRUE(bst.remove(20));
    EXPECT_FALSE(bst.contains(20));
    EXPECT_EQ(bst.size(), 4);

    // 2. Удаление узла с двумя потомками (30)
    EXPECT_TRUE(bst.remove(30));
    EXPECT_FALSE(bst.contains(30));
    EXPECT_TRUE(bst.contains(40));
    EXPECT_EQ(bst.size(), 3);

    // 3. Удаление корня (50)
    EXPECT_TRUE(bst.remove(50));
    EXPECT_FALSE(bst.contains(50));
    EXPECT_EQ(bst.size(), 2);

    // Удаление несуществующего элемента
    EXPECT_FALSE(bst.remove(999));
}


TEST(BSTTest, AllTraversalsVerification) {
    // Дерево:
    //        4
    //       / \
    //      2   6
    //     / \
    //    1   3
    BinarySearchTree<int> tree;
    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);

    // 1. LKP (In-order, по возрастанию)
    auto lkp = tree.traverse(TraversalOrder::LKP);
    EXPECT_EQ(lkp.size(), 5);
    EXPECT_EQ(lkp[0], 1); EXPECT_EQ(lkp[1], 2); EXPECT_EQ(lkp[2], 3);
    EXPECT_EQ(lkp[3], 4); EXPECT_EQ(lkp[4], 6);

    // 2. PKL (Reverse In-order, по убыванию)
    auto pkl = tree.traverse(TraversalOrder::PKL);
    EXPECT_EQ(pkl[0], 6); EXPECT_EQ(pkl[1], 4); EXPECT_EQ(pkl[2], 3);
    EXPECT_EQ(pkl[3], 2); EXPECT_EQ(pkl[4], 1);

    // 3. KLP (Pre-order)
    auto klp = tree.traverse(TraversalOrder::KLP);
    EXPECT_EQ(klp[0], 4); EXPECT_EQ(klp[1], 2); EXPECT_EQ(klp[2], 1);
    EXPECT_EQ(klp[3], 3); EXPECT_EQ(klp[4], 6);

    // 4. KPL (Root, Right, Left)
    auto kpl = tree.traverse(TraversalOrder::KPL);
    EXPECT_EQ(kpl[0], 4); EXPECT_EQ(kpl[1], 6); EXPECT_EQ(kpl[2], 2);
    EXPECT_EQ(kpl[3], 3); EXPECT_EQ(kpl[4], 1);

    // 5. LPK (Post-order)
    auto lpk = tree.traverse(TraversalOrder::LPK);
    EXPECT_EQ(lpk[0], 1); EXPECT_EQ(lpk[1], 3); EXPECT_EQ(lpk[2], 2);
    EXPECT_EQ(lpk[3], 6); EXPECT_EQ(lpk[4], 4);

    // 6. PLK (Right, Left, Root)
    auto plk = tree.traverse(TraversalOrder::PLK);
    EXPECT_EQ(plk[0], 6); EXPECT_EQ(plk[1], 3); EXPECT_EQ(plk[2], 1);
    EXPECT_EQ(plk[3], 2); EXPECT_EQ(plk[4], 4);

    // 7. BFS (Поуровневый)
    auto bfs = tree.traverse(TraversalOrder::BFS);
    EXPECT_EQ(bfs[0], 4); EXPECT_EQ(bfs[1], 2); EXPECT_EQ(bfs[2], 6);
    EXPECT_EQ(bfs[3], 1); EXPECT_EQ(bfs[4], 3);
}


TEST(BSTTest, MapReduceWhere) {
    BinarySearchTree<int> tree;
    for (int x : {1, 2, 3, 4, 5}) tree.insert(x);

    // map: преобразование int -> double
    auto mapped = tree.map([](int x) { return x * 1.5; });
    EXPECT_EQ(mapped.size(), 5);
    EXPECT_TRUE(mapped.contains(1.5));
    EXPECT_TRUE(mapped.contains(7.5));

    // where: фильтрация нечетных
    auto evens = tree.where([](int x) { return x % 2 == 0; });
    EXPECT_EQ(evens.size(), 2);
    EXPECT_TRUE(evens.contains(2));
    EXPECT_TRUE(evens.contains(4));
    EXPECT_FALSE(evens.contains(1));

    // reduce: подсчет суммы
    int sum = tree.reduce([](int acc, int x) { return acc + x; }, 0);
    EXPECT_EQ(sum, 15);
}


TEST(BSTTest, SubtreeExtractionAndSerialization) {
    BinarySearchTree<int> tree;
    for (int x : {50, 30, 70, 20, 40, 60, 80}) tree.insert(x);

    // Извлечение поддерева с корнем 30
    auto sub = tree.getSubtree(30);
    EXPECT_EQ(sub.size(), 3);
    EXPECT_TRUE(sub.contains(30));
    EXPECT_TRUE(sub.contains(20));
    EXPECT_TRUE(sub.contains(40));
    EXPECT_FALSE(sub.contains(50));
    EXPECT_THROW(tree.getSubtree(999), NodeNotFoundException);

    // Сериализация и восстановление
    std::string serialized = tree.serialize();
    auto restored = BinarySearchTree<int>::deserialize(serialized);

    EXPECT_EQ(tree.size(), restored.size());
    EXPECT_TRUE(tree == restored);
}


TEST(BSTTest, WorkWithStudentType) {
    BinarySearchTree<Student> tree;
    tree.insert(Student(200, "Ivan", "Ivanov", 2002, 4.5));
    tree.insert(Student(100, "Petr", "Petrov", 2003, 4.0));
    tree.insert(Student(300, "Anna", "Sidorova", 2001, 5.0));

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contains(Student(200, "", "", 0, 0.0)));

    // map: получение дерева фамилий
    auto lastNames = tree.map([](const Student& s) { return s.lastName; });
    EXPECT_TRUE(lastNames.contains("Ivanov"));
    EXPECT_TRUE(lastNames.contains("Petrov"));
    EXPECT_TRUE(lastNames.contains("Sidorova"));

    // reduce: средний балл
    double totalGpa = tree.reduce([](double acc, const Student& s) { return acc + s.gpa; }, 0.0);
    EXPECT_DOUBLE_EQ(totalGpa / tree.size(), (4.5 + 4.0 + 5.0) / 3.0);
}
