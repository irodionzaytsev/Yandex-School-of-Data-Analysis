#include <iostream>
#include <list>
#include <vector>

namespace mystd {
    template <typename It>
    It unique(It first, It last) {
        std::vector<It> unique_iters;
        unique_iters.push_back(first);
        It prev = first;
        for (auto current = std::next(first); current != last; ++current) {
            if (*current != *prev) {
                // std::cerr << *current << "\n";
                unique_iters.push_back(current);
            }
            prev = current;
        }
        int index = 0;
        auto current = first;
        for (; index < unique_iters.size(); ++current, ++index) {
            // std::cerr << *current << " " << *(unique_iters[index]) << "\n";
            std::swap(*current, *(unique_iters[index]));
        }
        return current;
    }

}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::list<int> sequence;

    int x;
    while (std::cin >> x) {
        sequence.push_back(x);
    }

    auto out_last = mystd::unique(sequence.begin(), sequence.end());

    for (auto it = sequence.begin(); it != out_last; ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
