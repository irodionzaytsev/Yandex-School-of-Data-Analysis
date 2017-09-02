#include <iostream>
#include <vector>
#include <cassert>

#include "stack.h"

#define ASSERT_EQ(expected, actual) { \
    if (expected != actual) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << actual << " (= " << #actual << ")" << std::endl; \
        std::terminate(); \
    } \
}

void TestEmptyStack() {
    Stack s;
    ASSERT_EQ(true, s.Empty());
    ASSERT_EQ(0, s.Size());
    ASSERT_EQ(false, s.Pop());
}

void TestSimplePushPop() {
    Stack s;
    s.Push(42);
    s.Push(43);

    ASSERT_EQ(2, s.Size());
    ASSERT_EQ(false, s.Empty());
    ASSERT_EQ(43, s.Top());

    ASSERT_EQ(true, s.Pop());
    ASSERT_EQ(1, s.Size());
    ASSERT_EQ(42, s.Top());
}

void TestStress() {
    Stack s;

    for (int i = 0; i < 1024; ++i) {
        s.Push(i);
    }

    for (int i = 1023; i >= 0; --i) {
        ASSERT_EQ(i, s.Top());
        ASSERT_EQ(true, s.Pop());
    }

    ASSERT_EQ(true, s.Empty());
}

int main() {
    TestEmptyStack();
    TestSimplePushPop();
    TestStress();

    return 0;
}
