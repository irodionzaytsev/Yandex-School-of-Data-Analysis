#include <cassert>
#include <iostream>

#include "static_map.h"

#define ASSERT_EQ(expected, actual) { \
    if (expected != actual) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << actual << " (= " << #actual << ")" << std::endl; \
        std::terminate(); \
    } \
}


void TestEmptyMap() {
    StaticMap map({});

    std::string value;
    ASSERT_EQ(false, map.Find("key", &value));
}

void TestSmallMap() {
    StaticMap map({ { "b", "1" }, { "a", "2" } });

    std::string value;
    ASSERT_EQ(true, map.Find("b", &value));
    ASSERT_EQ("1", value);

    ASSERT_EQ(true, map.Find("a", &value));
    ASSERT_EQ("2", value);

    ASSERT_EQ(false, map.Find("c", &value));
}

void TestSpeed() {
    const int NUM_ELEMENTS = 100000;

    std::vector<std::pair<std::string, std::string>> items;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        items.emplace_back(std::to_string(i), std::to_string(i));
    }

    std::mt19937 generator(42);
    std::shuffle(items.begin(), items.end(), generator);

    StaticMap map(items);

    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        std::string value;
        ASSERT_EQ(true, map.Find(std::to_string(i), &value));
        ASSERT_EQ(std::to_string(i), value);
    }
}

int main() {
    TestEmptyMap();
    TestSmallMap();
    TestSpeed();

    return 0;
}
