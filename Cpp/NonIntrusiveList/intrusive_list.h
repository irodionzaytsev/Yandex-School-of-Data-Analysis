#pragma once
#include <iostream>
#include <algorithm>

class ListHook {
public:
    ListHook();

    bool IsLinked() const;

    void Unlink();

    // Must unlink element from list
    ~ListHook();

    ListHook(const ListHook&) = delete;

private:
    template<class T>
    friend class List;

    // that helper function might be useful
    void LinkBefore(ListHook* other);

    ListHook* next;
    ListHook* previous;

    // template<class T>
    // List<T>* owner_list;
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator& operator++();
        Iterator operator++(int);

        T& operator*() const;
        T* operator->() const;

        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;

        Iterator(ListHook* value);
    private:
        ListHook* value_;

    };

    List();
    List(const List&) = delete;
    List(List&& other);

    // must unlink all elements from list
    ~List();

    List& operator=(const List&) = delete;
    List& operator=(List&& other);

    bool IsEmpty() const;
    size_t Size() const;

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem);
    void PushFront(T* elem);

    T& Front();
    const T& Front() const;

    T& Back();
    const T& Back() const;

    void PopBack();
    void PopFront();

    Iterator Begin();
    Iterator Begin() const;
    Iterator End();
    Iterator End() const;

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element);

private:
    ListHook dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) { return list.Begin(); }

template <typename T>
typename List<T>::Iterator end(List<T>& list) { return list.End(); }

ListHook::ListHook() {
    previous = nullptr;
    next = nullptr;
    // owner_list = nullptr;
}

bool ListHook::IsLinked() const {
    return (next != nullptr);
}

void ListHook::Unlink() {
    // owner_list = nullptr;
    if (previous != nullptr) {
        previous->next = next;

    }
    if (next != nullptr) {
        next->previous = previous;

    }
    previous = nullptr;
    next = previous;
}

ListHook::~ListHook() {
    Unlink();
}

void ListHook::LinkBefore(ListHook *other) {
    other->previous = previous;
    other->next = this;
    previous->next = other;
    previous = other;
}

template <typename T>
List<T>::List() {
    dummy_.next = &dummy_;
    dummy_.previous = &dummy_;
};

template <typename T>
List<T>::List(List &&other) {
    dummy_.next = other.dummy_.next;
    other.dummy_.next->previous = &dummy_;
    dummy_.previous = other.dummy_.previous;
    other.dummy_.previous->next = &dummy_;
    other.dummy_.next = &other.dummy_;
    other.dummy_.previous = &other.dummy_;
}

template <typename T>
List<T>::~List() {
    while(dummy_.next != &dummy_) {
        auto tmp_ptr = dummy_.next;
        tmp_ptr->Unlink();
    }
}

template <typename T>
List<T>& List<T>::operator=(List &&other) {
    dummy_.next = other.dummy_.next;
    other.dummy_.next->previous = &dummy_;
    dummy_.previous = other.dummy_.previous;
    other.dummy_.previous->next = &dummy_;
    other.dummy_.next = &other.dummy_;
    other.dummy_.previous = &other.dummy_;
    return *this;
}

template <typename T>
bool List<T>::IsEmpty() const {
    return (dummy_.next == &dummy_);
}

template <typename T>
size_t List<T>::Size() const {
    size_t size = 0;
    for (ListHook* listhook_ptr = dummy_.next; listhook_ptr != &dummy_; listhook_ptr = listhook_ptr->next) {
        ++size;
    }
    return size;
}

template <typename T>
void List<T>::PushBack(T *elem) {
    dummy_.LinkBefore(elem);
}

template <typename T>
void List<T>::PushFront(T *elem) {
    dummy_.next->LinkBefore(elem);
}

template <typename T>
T& List<T>::Front() {
    return *(static_cast<T*>(dummy_.next));
}

template <typename T>
const T& List<T>::Front() const {
    return *(static_cast<T*>(dummy_.next));
}

template <typename T>
T& List<T>::Back() {
    return *(static_cast<T*>(dummy_.previous));
}

template <typename T>
const T& List<T>::Back() const {
    return *(static_cast<T*>(dummy_.previous));
}

template <typename T>
void List<T>::PopBack() {
    dummy_.previous->Unlink();
}

template <typename T>
void List<T>::PopFront() {
    dummy_.next->Unlink();
}

template <typename T>
typename List<T>::Iterator List<T>::IteratorTo(T* element) {
    return Iterator(element);
}

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    value_ = static_cast<T*>(value_->next);
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int pos) {
    for (int i = 0; i < pos; ++i) {
        value_ = static_cast<T*>(value_->next);
    }
}

template <typename T>
bool List<T>::Iterator::operator!=(const Iterator &rhs) const {
    return (value_ != rhs.value_);
}

template <typename T>
T& List<T>::Iterator::operator*() const {
    return *static_cast<T*>(value_);
}

template <typename T>
T* List<T>::Iterator::operator->() const {
    return static_cast<T*>(value_);
}

template <typename T>
bool List<T>::Iterator::operator==(const Iterator &rhs) const {
    return (value_ == rhs.value_);
}

template <typename T>
List<T>::Iterator::Iterator(ListHook* value) {
    value_ = value;
}

template <typename T>
typename List<T>::Iterator List<T>::Begin(){
    return Iterator(dummy_.next);
}

template <typename T>
typename List<T>::Iterator List<T>::Begin() const {
    return Iterator(dummy_.next);
}

template <typename T>
typename List<T>::Iterator List<T>::End(){
    return Iterator(&dummy_);
}

template <typename T>
typename List<T>::Iterator List<T>::End() const {
    return Iterator(&dummy_);
}