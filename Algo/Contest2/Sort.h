#include <iostream>
#include <vector>
#include <algorithm>
/*
template<typename T, typename _Compare>
class HeapPtr {

public:
    HeapPtr(_Compare __comp);

    void add(T* element);

    T* getMin() const;

    void pop();

    int getSize() const;

    bool isEmpty() const;

private:
    int size_;
    std::vector<T*> elements_;

     // we suppose that comp_(a, b)
     // is -1 if a < b,
     // is  0 if a = b,
     // is  1 if a > b

    _Compare comp_;
    bool heapify(int index);
};

template <typename T, typename _Compare>
HeapPtr<T*>::HeapPtr(_Compare __comp) {
    size_ = 0;
    comp_ = __comp;
}

template<typename T>
void HeapPtr<T*>::add(const T* element) {
    ++size_;
    elements_.push_back(element);
    if (size_ == 1) {
        return;
    }
    bool flag = true;
    int current_index = (size_ - 2) / 2;
    while (flag) {
        flag = this->heapify(current_index);
        current_index = (current_index - 1) / 2;
    }
}

template<typename T>
T* HeapPtr<T*>::getMin() const {
    return elements_[0];
}

template<typename T>
void HeapPtr<T*>::pop() {
    std::swap(elements_[0], elements_[size_ - 1]);
    elements_.pop_back();
    --size_;
    heapify(0);
}

template<typename T>
int HeapPtr<T*>::getSize() const {
    return size_;
}

template<typename T>
bool HeapPtr<T*>::isEmpty() const {
    return elements_.empty();
}

template<typename T, typename _Compare>
bool HeapPtr<T*>::heapify(const int index) {
    if (2 * index + 1 >= size_) {
        return false;
    }
    if (2 * index + 2 >= size_) {
        if (comp_(*elements_[index], *elements_[2 * index + 1]) > 0)  {
            std::swap(elements_[index], elements_[2 * index + 1]);
            return true;
        }
        return false;
    }
    if (comp_(*elements_[index], *elements_[2 * index+1]) <= 0 &&
        comp_(*elements_[index], *elements_[2 * index + 2]) <= 0) {
        return false;
    }
    if (comp_(*elements_[2 * index + 1], *elements_[2 * index + 2]) < 0) {
        std::swap(elements_[index], elements_[2 * index + 1]);
        heapify(2 * index + 1);
        return true;
    } else {
        std::swap(elements_[index], elements_[2 * index + 2]);
        heapify(2 * index + 2);
        return true;
    }
}

template<typename _RandomAccessIterator, typename _Compare>
inline void
heapSort(_RandomAccessIterator __first,
         _RandomAccessIterator __last,
         _Compare __comp) {
    HeapPtr<typename std::iterator_traits<__first>::value_type> heapPtr(__comp);


    for (auto current_it = __first; current_it != __last; ++__current_it) {
        heapPtr.add(current_it);
    }

    auto current_it = __first;

    _RandomAccessIterator next = current_it;
    while (next != __last) {
        next = current_it + 1;
        std::swap(heapPtr.getMin(), current_it);
    }

    while (!(*elements).empty()) {
        heap.add((*elements).back());
        (*elements).pop_back();
    }
    while (!heap.isEmpty()) {
        (*elements).push_back(heap.getMin());
        heap.pop();
    }
}
*/

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
        effectiveness_ (_effectiveness),
        index_ (_index) {};
/*
int cmpEffectiveness(const Player& first_player, const Player& second_player) {
    if (first_player.getEffectiveness() < second_player.getEffectiveness()) {
        return -1;
    }
    if (second_player.getEffectiveness() > second_player.getEffectiveness()) {
        return 1;
    }
    return 0;
}
*/
int main() {

    /*std::vector<Player> players;
    for (int i = 0; i < 10; ++i) {
        players.push_back(new Player(rand() % 10, rand() % 10));
    }

    for (auto& it : players) {
        delete it;
    }*/

    std::vector<int> v;
    for (int i = 0; i < 20000; ++i) {
        v.push_back(rand() % 1000);
    }

    const clock_t begin_time = clock();

    auto it = v.end();
    // typename std::iterator_traits<>::value_type it = v.begin();
    while (it != v.begin()) {
        std::make_heap(v.begin(), it);
/*
        for (auto itt = v.begin(); itt != v.end(); ++itt) {
            std::cerr << *itt << " ";
        }
        std::cerr << "\n";
        */
        --it;
        auto begin = v.begin();
        std::swap(*begin, *it);
    }

    std::cerr << float( clock () - begin_time ) /  CLOCKS_PER_SEC;

    // std::cerr << "result\n";

    // for (auto it = v.begin(); it != v.end(); ++it) {
    //     std::cerr << *it << " ";
    // }
    // std::cerr << "\n";

    // std::sort(players.begin(), players.end(), cmpEffectiveness);

    return 0;
}

template<typename _RandomAccessIterator, typename _Distance, typename _Tp, typename _Compare>
void __adjust_heap(_RandomAccessIterator __first,
                   _Distance __holeIndex,
                   _Distance __len,
                   _Tp __value,
                   _Compare __comp)
{
    const _Distance __topIndex = __holeIndex;
    _Distance __secondChild = __holeIndex;
    while (__secondChild < (__len - 1) / 2) {
        __secondChild = 2 * (__secondChild + 1);
        if (__comp(__first + __secondChild, __first + (__secondChild - 1))) {
            __secondChild--;
        }
        *(__first + __holeIndex) = _GLIBCXX_MOVE(*(__first + __secondChild));
        __holeIndex = __secondChild;
    }
    if ((__len & 1) == 0 && __secondChild == (__len - 2) / 2) {
        __secondChild = 2 * (__secondChild + 1);
        *(__first + __holeIndex) = _GLIBCXX_MOVE(*(__first
                                                   + (__secondChild - 1)));
        __holeIndex = __secondChild - 1;
    }
    std::__push_heap(__first,
                     __holeIndex,
                     __topIndex,
                     _GLIBCXX_MOVE(__value),
                     __gnu_cxx::__ops::__iter_comp_val(__comp));
}