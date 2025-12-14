#include "gtest.h"
#include "stack.h"
#include <stdexcept>
#include <string>

TEST(TStackTest, CanCreateStackWithPositiveCapacity) {
    ASSERT_NO_THROW(TStack<int> s(10));
}

TEST(TStackTest, ThrowsWhenCreatingStackWithZeroCapacity) {
    ASSERT_ANY_THROW(TStack<int> s(0));
}

TEST(TStackTest, NewStackIsEmpty) {
    TStack<int> s(5);
    EXPECT_TRUE(s.IsEmpty());
    EXPECT_EQ(0, s.GetCount());
}

TEST(TStackTest, IsFullReturnsTrueForFullStack) {
    TStack<int> s(2);
    s.Push(1);
    s.Push(2);
    EXPECT_TRUE(s.IsFull());
}

TEST(TStackTest, CanPushElement) {
    TStack<double> s(5);
    ASSERT_NO_THROW(s.Push(3.14));
    EXPECT_EQ(1, s.GetCount());
}

TEST(TStackTest, ThrowsWhenPushingToFullStack) {
    TStack<int> s(1);
    s.Push(100);
    ASSERT_ANY_THROW(s.Push(200));
}

TEST(TStackTest, CanPopElement) {
    TStack<int> s(5);
    s.Push(42);
    EXPECT_EQ(42, s.Pop());
    EXPECT_TRUE(s.IsEmpty());
}

TEST(TStackTest, PopReturnsElementsInLIFOOrder) {
    TStack<char> s(3);
    s.Push('A');
    s.Push('B');
    s.Push('C');
    EXPECT_EQ('C', s.Pop());
    EXPECT_EQ('B', s.Pop());
    EXPECT_EQ('A', s.Pop());
}

TEST(TStackTest, ThrowsWhenPopFromEmptyStack) {
    TStack<int> s(5);
    ASSERT_ANY_THROW(s.Pop());
}

TEST(TStackTest, CanInspectTopElementWithoutChangingCount) {
    TStack<std::string> s(5);
    std::string val = "TestValue";
    s.Push(val);
    size_t initialCount = s.GetCount();

    EXPECT_EQ(val, s.Top());
    EXPECT_EQ(initialCount, s.GetCount());
}

TEST(TStackTest, ThrowsWhenTopFromEmptyStack) {
    TStack<int> s(5);
    ASSERT_ANY_THROW(s.Top());
}