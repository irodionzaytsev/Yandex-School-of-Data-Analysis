#pragma once
#include <iostream>
#include <vector>

class Stack {
 public:
    void Push(int x);
    bool Pop();

    int Top() const;
    bool Empty() const;
    size_t Size() const;

 private:
    std::vector<int> container_;
};

void Stack::Push(int x) {
    container_.push_back(x);
}

bool Stack::Pop() {
    if (container_.size() > 0) {
        container_.pop_back();
        return true;
    }
    return false;
}

int Stack::Top() const {
    return container_.back();
}

size_t Stack::Size() const {
    return container_.size();
}

bool Stack::Empty() const {
    return container_.empty();
}
