#include <iostream>
#include <cassert>

#include "lru_cache.h"

#define ASSERT_EQ(expected, actual) { \
    auto __expected = expected; \
    auto __actual = actual; \
    if (__expected != __actual) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << __expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl; \
        std::terminate(); \
    } \
}

void TestSetGet() {
    LruCache cache(10);

    std::string value;

    cache.Set("a", "1");
    cache.Set("b", "2");
    cache.Set("c", "3");

    ASSERT_EQ(true, cache.Get("a", &value));
    ASSERT_EQ("1", value);
    ASSERT_EQ(true, cache.Get("b", &value));
    ASSERT_EQ("2", value);
    ASSERT_EQ(true, cache.Get("c", &value));
    ASSERT_EQ("3", value);

    ASSERT_EQ(false, cache.Get("d", &value));

    cache.Set("c", "4");
    ASSERT_EQ(true, cache.Get("c", &value));
    ASSERT_EQ("4", value);
}

void TestEviction() {
    LruCache cache(2);
    std::string value;

    cache.Set("a", "1");
    cache.Set("b", "2");
    cache.Set("c", "3");

    ASSERT_EQ(false, cache.Get("a", &value));
    ASSERT_EQ(true, cache.Get("b", &value));
    ASSERT_EQ(true, cache.Get("c", &value));

    cache.Set("b", "4");
    cache.Set("c", "5");
    cache.Set("b", "6");

    cache.Set("e", "7");
    ASSERT_EQ(false, cache.Get("c", &value));
    ASSERT_EQ(true, cache.Get("b", &value));
    ASSERT_EQ(true, cache.Get("e", &value));

    cache.Get("b", &value);
    cache.Set("f", "8");
    ASSERT_EQ(false, cache.Get("e", &value));
    ASSERT_EQ(true, cache.Get("b", &value));
    ASSERT_EQ(true, cache.Get("f", &value));
}


void TestStress() {
    LruCache cache(100);
    std::string value;

    srand(42);
    for (size_t i = 0; i < 100000; ++i) {
        if (rand() % 2 == 0) {
            cache.Set(std::to_string(rand() % 500), "foo");
        } else {
            cache.Get(std::to_string(rand() % 500), &value);
        }
    }
}

int main() {
    TestSetGet();
    TestEviction();
    TestStress();
    return 0;
}
