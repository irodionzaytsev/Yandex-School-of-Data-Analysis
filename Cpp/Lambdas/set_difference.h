#include <iostream>
#include <list>
#include <vector>

namespace mystd {
    template <typename It1, typename It2, typename Out>
    Out set_difference(It1 first1, It1 last1, It2 first2, It2 last2, Out out) {
        auto mid2 = first2;
        for (auto mid1 = first1; mid1 != last1; ++mid1) {
            while (mid2 != last2 && *mid2 < *mid1) {
                ++mid2;
            }
            if (mid2 == last2 || *mid2 != *mid1) {
                *out = *mid1;
                ++out;
            } else if (mid2 != last2) {
                ++mid2;
            }
        }
        return out;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::list<int64_t> sequence;

    int first;
    std::cin >> first;
    std::vector<int> firsts(first);
    for (int i = 0; i < first; ++i) {
        std::cin >> firsts[i];
    }

    int second;
    std::cin >> second;
    std::vector<int> seconds(second);
    for (int i = 0; i < second; ++i) {
        std::cin >> seconds[i];
    }

    std::vector<int> thirds(first + second);

    auto out_last = mystd::set_difference(firsts.begin(),
                                          firsts.end(),
                                          seconds.begin(),
                                          seconds.end(),
                                          thirds.begin());
    for (auto it = thirds.begin(); it != out_last; ++it) {
        std::cout << *it << " ";
    }


    return 0;
}
