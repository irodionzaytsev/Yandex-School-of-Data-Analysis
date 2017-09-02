#include <iostream>
#include <list>
#include <vector>

namespace mystd {
    template <typename It, typename Out>
    Out reverse_copy(It first, It last, Out out) {
        auto it = last;
        while (it != first) {
            --it;
            *out = *it;
            ++out;
        }
        return out;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::list<int> sequence;

    int x;
    while (std::cin >> x) {
        sequence.push_back(x);
    }

    std::vector<int> rev_sequence(sequence.size());

    auto out_last = mystd::reverse_copy(sequence.begin(), sequence.end(), rev_sequence.begin());

    for (auto it = rev_sequence.begin(); it != out_last; ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
