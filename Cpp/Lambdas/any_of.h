#include <iostream>
#include <list>
#include <vector>

namespace mystd {
    template <typename It, typename Pred>
    bool any_of(It first, It last, Pred f) {
        for (It tmp = first; tmp != last; ++tmp) {
            if (f(*tmp)) {
                return true;
            }
        }
        return false;
    }
}

bool isPrime(int64_t x) {
    if (x <= 1) {
        return false;
    }
    for (int64_t n = 2; n * n <= x; ++n) {
        if (x % n == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::list<int64_t> sequence;

    int64_t x;
    while (std::cin >> x) {
        sequence.push_back(x);
    }

    if (mystd::any_of(sequence.begin(), sequence.end(), isPrime)) {
        std::cout << "true\n";
    } else {
        std::cout << "false\n";
    }


    return 0;
}
