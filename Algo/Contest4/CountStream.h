#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>

class CountingDict {
public:
    CountingDict() {
        seed = 343232;
        bucket.assign(MAX, false);
    }

    int32_t calcNumber(int32_t left, int32_t right) const {
        int32_t degree = 1;
        int32_t result = 0;
        for (int i = left; i < right; ++i) {
            if (bucket[i]) {
                result += degree;
            }
            degree *= 2;
        }
        return result;
    }

    void setNumber(const int32_t left, const int32_t right, int32_t number) {
        for (int i = left; i < right; ++i) {
            bucket[i] = (number % 2 == 1);
            number /= 2;
        }
    }

    void Insert(int32_t element) {
        int32_t hash = calcHash(element);
        int32_t number = calcNumber(hash * BITS, hash * BITS + BITS) + 1;
        setNumber(hash * BITS, hash * BITS + BITS, number);
        // ++bucket[];
    }

    int GetCount(int32_t element) const {
        int32_t hash = calcHash(element);
        int32_t number = calcNumber(hash * BITS, hash * BITS + BITS);
        return number;
    }

private:
    int32_t calcHash(int32_t element) const {
        int64_t element_copy = element;
        int32_t hash = int32_t(((element_copy * seed) % KEK) % PRIME);
        if (hash < 0) {
            hash = -hash + 1;
        }
        return hash;
    }

    static const int64_t MAX = 7 * 1200003;
    static const int64_t BITS = 7;
    static const int64_t PRIME = MAX / BITS;
    static const int64_t KEK = 1000000009;
    std::vector<bool> bucket;
    int64_t seed;
};
