#include <iostream>
#include <vector>
#include <algorithm>
#include <typeinfo>

template<typename _RandomAccessIterator, typename _Compare>
inline bool
heapify(_RandomAccessIterator first, const int index, const int len, const _Compare comp) {

    if (2 * index + 1 >= len) {
        return false;
    }

    if (2 * index + 2 >= len) {
        if (comp(*(first + index), *(first + 2 * index + 1))) {
            std::swap(*(first + index), *(first + 2 * index + 1));
            return true;
        }
        return false;
    }

    if (!comp(*(first + index), *(first + 2 * index + 1)) &&
        !comp(*(first + index), *(first + 2 * index + 2))) {
        return false;
    }

    if (!comp(*(first + 2 * index + 1), *(first + 2 * index + 2))) {
        std::swap(*(first + index), *(first + 2 * index + 1));
        heapify(first, 2 * index + 1, len, comp);
        return true;
    } else {
        std::swap(*(first + index), *(first + 2 * index + 2));
        heapify(first, 2 * index + 2, len, comp);
        return true;
    }
}

template<typename _RandomAccessIterator, typename _Compare>
inline void
makeHeap(_RandomAccessIterator first,
         _RandomAccessIterator last,
         _Compare comp) {
    int len = last - first;
    for (int index = len - 1; index >= 0; --index) {
        heapify(first, index, len, comp);
    }
}

template<typename _RandomAccessIterator, typename _Compare>
inline void
heapSort(_RandomAccessIterator first,
         _RandomAccessIterator last,
         _Compare comp) {

    makeHeap(first, last, comp);
    int len = last - first;
    for (int index = len - 1; index >= 0; --index) {
        std::swap(*(first), *(first + index));
        heapify(first, 0, index, comp);
    }
}

class Player {

public:
    Player(long long _effectiveness, int _index);

    long long getEffectiveness() const;

    int getIndex() const;

private:
    long long effectiveness_;
    int index_;
};

Player::Player(long long _effectiveness, int _index) :
        effectiveness_(_effectiveness),
        index_(_index) {};

long long Player::getEffectiveness() const {
    return effectiveness_;

}

int Player::getIndex() const {
    return index_;
}

bool cmp(const Player& first, const Player& second) {
    return (first.getEffectiveness() < second.getEffectiveness());
}


int main() {

    std::vector<Player> v;
    for (int i = 0; i < 20; ++i) {
        v.push_back(Player(rand() % 10, rand() % 10));
    }

    const clock_t begin_time = clock();
    heapSort(v.begin(), v.end(), cmp);
    std::cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;

    for (auto& x : v) {
        std::cerr << x.getEffectiveness() << " : " << x.getIndex() << "\n";
    }
    std::cerr << "\n";


    return 0;
}


