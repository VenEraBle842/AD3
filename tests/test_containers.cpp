#include <gtest/gtest.h>

#include "../containers/dynamic_array.hpp"
#include "../containers/stack.hpp"
#include "../containers/queue.hpp"

#include <string>

TEST(DynamicArrayTest, PushPopAndReallocate) {
    DynamicArray<int> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0);

    for (int i = 0; i < 50; ++i) {
        arr.push_back(i);
    }
    EXPECT_EQ(arr.size(), 50);

    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(arr[i], i);
    }

    arr.pop_back();
    EXPECT_EQ(arr.size(), 49);
    EXPECT_THROW(arr.at(100), std::out_of_range);
}

TEST(DynamicArrayTest, RangeBasedForIteration) {
    DynamicArray<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.push_back(i * 10);
    }

    int expected = 10;
    for (int val : arr) {
        EXPECT_EQ(val, expected);
        expected += 10;
    }
    EXPECT_EQ(expected, 60);
}

TEST(DynamicArrayTest, InitializerListAndCopy) {
    DynamicArray<std::string> arr{"one", "two", "three"};
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "one");
    EXPECT_EQ(arr[2], "three");

    DynamicArray<std::string> copy = arr;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[1], "two");

    copy[1] = "changed";
    EXPECT_EQ(arr[1], "two"); // Deep copy check
    EXPECT_EQ(copy[1], "changed");
}


TEST(StackTest, PushPopTopUnderflow) {
    Stack<std::string> st;
    EXPECT_TRUE(st.empty());

    st.push("alpha");
    st.push("beta");
    st.push("gamma");

    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.top(), "gamma");

    st.pop();
    EXPECT_EQ(st.top(), "beta");
    st.pop();
    st.pop();

    EXPECT_TRUE(st.empty());
    EXPECT_THROW(st.pop(), std::underflow_error);
    EXPECT_THROW(st.top(), std::underflow_error);
}

TEST(StackTest, ClearOperation) {
    Stack<int> st;
    for (int i = 0; i < 10; ++i) st.push(i);
    EXPECT_EQ(st.size(), 10);

    st.clear();
    EXPECT_TRUE(st.empty());
    EXPECT_EQ(st.size(), 0);
}


TEST(QueueTest, RingBufferBehaviorAndWrapAround) {
    Queue<int> q;
    EXPECT_TRUE(q.empty());

    for (int i = 1; i <= 5; ++i) q.push(i);
    EXPECT_EQ(q.size(), 5);
    EXPECT_EQ(q.front(), 1);

    // Удаляем 2 элемента
    EXPECT_EQ(q.front(), 1); q.pop();
    EXPECT_EQ(q.front(), 2); q.pop();

    // Добавляем элементы для зацикливания кольцевого буфера
    q.push(6);
    q.push(7);

    EXPECT_EQ(q.front(), 3); q.pop();
    EXPECT_EQ(q.front(), 4); q.pop();
    EXPECT_EQ(q.front(), 5); q.pop();
    EXPECT_EQ(q.front(), 6); q.pop();
    EXPECT_EQ(q.front(), 7); q.pop();

    EXPECT_TRUE(q.empty());
    EXPECT_THROW(q.pop(), std::underflow_error);
    EXPECT_THROW(q.front(), std::underflow_error);
}

TEST(QueueTest, CopyAndMove) {
    Queue<int> q1;
    q1.push(10);
    q1.push(20);

    Queue<int> q2 = q1;
    EXPECT_EQ(q2.size(), 2);
    EXPECT_EQ(q2.front(), 10);
    q2.pop();
    EXPECT_EQ(q2.front(), 20);
    EXPECT_EQ(q1.front(), 10); // Исходная очередь не изменилась
}
