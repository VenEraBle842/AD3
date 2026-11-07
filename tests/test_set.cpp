#include <gtest/gtest.h>

#include "../set/set.hpp"
#include "../tree/bst_exceptions.hpp"
#include "../models/student.hpp"

#include <string>

TEST(SetTest, BasicOperationsAndAlgebra) {
    Set<int> A{1, 2, 3, 4};
    Set<int> B{3, 4, 5, 6};

    // Объединение (A + B) -> {1, 2, 3, 4, 5, 6}
    Set<int> unionSet = A + B;
    EXPECT_EQ(unionSet.size(), 6);
    for (int i = 1; i <= 6; ++i) {
        EXPECT_TRUE(unionSet.contains(i));
    }

    // Пересечение (A * B) -> {3, 4}
    Set<int> interSet = A * B;
    EXPECT_EQ(interSet.size(), 2);
    EXPECT_TRUE(interSet.contains(3));
    EXPECT_TRUE(interSet.contains(4));
    EXPECT_FALSE(interSet.contains(1));

    // Разность (A - B) -> {1, 2}
    Set<int> diffSet = A - B;
    EXPECT_EQ(diffSet.size(), 2);
    EXPECT_TRUE(diffSet.contains(1));
    EXPECT_TRUE(diffSet.contains(2));
    EXPECT_FALSE(diffSet.contains(3));

    // Проверка подмножества (A <= B)
    Set<int> sub{3, 4};
    EXPECT_TRUE(sub <= A);
    EXPECT_TRUE(sub <= B);
    EXPECT_FALSE(A <= B);

    // Равенство
    Set<int> duplicateA{4, 3, 2, 1};
    EXPECT_TRUE(A == duplicateA);
    EXPECT_FALSE(A == B);
}


TEST(SetTest, FunctionalOpsAndSerialization) {
    Set<std::string> set{"apple", "banana", "cherry"};

    // where: слова длиннее 5 букв
    auto longWords = set.where([](const std::string& s) { return s.length() > 5; });
    EXPECT_EQ(longWords.size(), 2);
    EXPECT_TRUE(longWords.contains("banana"));
    EXPECT_TRUE(longWords.contains("cherry"));
    EXPECT_FALSE(longWords.contains("apple"));

    // map: получение длин слов
    auto lengths = set.map([](const std::string& s) { return static_cast<int>(s.length()); });
    EXPECT_TRUE(lengths.contains(5)); // "apple"
    EXPECT_TRUE(lengths.contains(6)); // "banana", "cherry" (дубликат длины схлопнут)
    EXPECT_EQ(lengths.size(), 2);

    // Сериализация и десериализация
    std::string s = set.serialize();
    auto restored = Set<std::string>::deserialize(s);

    EXPECT_EQ(set.size(), restored.size());
    EXPECT_TRUE(set == restored);

    // Ошибочный формат строки
    EXPECT_THROW(Set<int>::deserialize("invalid string without brackets"), ParseException);
}

TEST(SetTest, SetWithStudents) {
    Set<Student> students{
        Student(1, "A", "AA", 2000, 4.0),
        Student(2, "B", "BB", 2001, 4.5)
    };

    EXPECT_EQ(students.size(), 2);
    EXPECT_TRUE(students.contains(Student(1, "", "", 0, 0.0)));

    students.remove(Student(1, "", "", 0, 0.0));
    EXPECT_EQ(students.size(), 1);
    EXPECT_FALSE(students.contains(Student(1, "", "", 0, 0.0)));
}
