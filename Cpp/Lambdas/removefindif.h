#include <iostream>
#include <list>
namespace mystd {
    template <typename It, typename Out, typename Pred>
    Out remove_copy_if(It first, It last, Out out, Pred f) {
        for (It mid = first; mid != last; ++mid) {
            if (!f(*mid)) {
                *out = *mid;
                ++out;
            }
        }
        return out;
    }
}

bool isEven(int element) {
    return (element % 2 == 0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::list<int> first_list;
    int x;
    while (std::cin >> x) {
        first_list.push_back(x);
    }
    std::list<int> second_list(first_list.size());

    auto out_last = mystd::remove_copy_if(first_list.begin(),
                                          first_list.end(),
                                          second_list.begin(),
                                          isEven);

    for (auto it = second_list.begin(); it != out_last; ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
