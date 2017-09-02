#include <iostream>

#include "ring_buffer.h"

#define ASSERT_EQ(expected, actual) { \
    if (expected != actual) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << actual << " (= " << #actual << ")" << std::endl; \
        std::terminate(); \
    } \
}

void TestEmpty() {
    RingBuffer buffer(1);

    ASSERT_EQ(0, buffer.Size());
    ASSERT_EQ(true, buffer.Empty());
}

void TestPushAndPop() {
    RingBuffer buffer(2);

    int i;
    ASSERT_EQ(true, buffer.TryPush(0));
    ASSERT_EQ(true, buffer.TryPush(1));
    ASSERT_EQ(false, buffer.TryPush(2));

    ASSERT_EQ(2, buffer.Size());
    ASSERT_EQ(false, buffer.Empty());

    ASSERT_EQ(true, buffer.TryPop(&i));
    ASSERT_EQ(0, i);
    ASSERT_EQ(true, buffer.TryPop(&i));
    ASSERT_EQ(1, i);

    ASSERT_EQ(false, buffer.TryPop(&i));
    ASSERT_EQ(0, buffer.Size());
    ASSERT_EQ(true, buffer.Empty());
}

void TestRandom() {
    srand(42);

    RingBuffer buffer(10);

    int next_element = 0;
    int next_received_element = 0;
    for (int i = 0; i < 100000; ++i) {
        if (rand() % 2 == 0) {
            if (buffer.TryPush(next_element)) {
                next_element++;
            }
        } else {
            int element;
            if (buffer.TryPop(&element)) {
                ASSERT_EQ(next_received_element, element);
                next_received_element++;
            }
        }
    }
}

int main() {
    TestEmpty();
    TestPushAndPop();
    TestRandom();
    return 0;
}
