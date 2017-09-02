#pragma once
#include <iostream>
#include <deque>
class RingBuffer {
 public:
    explicit RingBuffer(size_t capacity);

    size_t Size() const;
    bool Empty() const;

    bool TryPush(int element);

    bool TryPop(int* element);

 private:
    std::deque<int> buffer_;
    int capacity_;
};

RingBuffer::RingBuffer(size_t capacity) {
    capacity_ = capacity;
}

size_t RingBuffer::Size() const {
    return buffer_.size();
}

bool RingBuffer::Empty() const {
    return buffer_.empty();
}

bool RingBuffer::TryPush(int element) {
    if (buffer_.size() + 1 <= capacity_) {
        buffer_.push_back(element);
        return true;
    }
    return false;
}

bool RingBuffer::TryPop(int *element) {
    if (buffer_.size() > 0) {
        (*element) = buffer_.front();
        buffer_.pop_front();
        return true;
    }
    return false;
}
