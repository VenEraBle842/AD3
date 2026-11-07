#include <gtest/gtest.h>

#include "../models/student.hpp"
#include <sstream>

TEST(StudentTest, ComparisonOperators) {
    Student s1(101, "Ivan", "Ivanov", 2003, 4.8);
    Student s2(102, "Petr", "Petrov", 2002, 4.2);
    Student s3(101, "Ivan", "Ivanov", 2003, 4.8);
    Student s4(100, "Anna", "Smirnova", 2004, 5.0);

    // Упорядочивание строго по id
    EXPECT_TRUE(s1 < s2);
    EXPECT_TRUE(s4 < s1);
    EXPECT_FALSE(s2 < s1);

    EXPECT_TRUE(s2 > s1);
    EXPECT_TRUE(s1 <= s3);
    EXPECT_TRUE(s1 >= s3);

    // Полное равенство по всем полям
    EXPECT_TRUE(s1 == s3);
    EXPECT_FALSE(s1 == s2);
    EXPECT_TRUE(s1 != s2);
}

TEST(StudentTest, StreamSerialization) {
    Student original(205, "Elena", "Kozlova", 2001, 4.95);

    std::stringstream ss;
    ss << original;
    EXPECT_EQ(ss.str(), "205:Elena:Kozlova:2001:4.95");

    Student restored;
    ss >> restored;
    EXPECT_EQ(original, restored);
    EXPECT_EQ(restored.id, 205);
    EXPECT_EQ(restored.firstName, "Elena");
    EXPECT_EQ(restored.lastName, "Kozlova");
    EXPECT_EQ(restored.birthYear, 2001);
    EXPECT_DOUBLE_EQ(restored.gpa, 4.95);
}
