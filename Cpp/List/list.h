#pragma once

#include <glob.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>

template <typename T>
class Node{
public:
    Node() {
        left_ptr_ = nullptr;
        right_ptr_ = nullptr;
    };

    Node(T _value) {
        value_ptr_.reset(&_value);
        left_ptr_ = nullptr;
        right_ptr_ = nullptr;
    }

    void setLeftPtr(Node* ptr) {
        left_ptr_ = ptr;
    }

    void setRightPtr(Node* ptr) {
        right_ptr_ = ptr;
    }

    Node* getLeft() const {
        return left_ptr_;
    }

    Node* getRight() const {
        return right_ptr_;
    }

    T& getValue() {
        return *value_ptr_;
    }


/*
    void setValue(const T& _value) {
        std::unique_ptr<T> uniq(new T(&_value));
        value_ptr_= std::move(uniq);
        //value_ptr_.reset(std::unique_ptr<T>(&_value));
        //value_ptr_ = &_value;
    }
*/

    void setValue(T&& _value) {
        std::unique_ptr<T> uniq(&_value);
        value_ptr_ = std::move(uniq);
        //value_ptr_ = std::move(std::unique_ptr<T>(_value));
        //value_ptr_.reset(_value);
        //value_ptr_ = std::move(_value);
    }
/*
    void setValue(T&& _value) {
        std::unique_ptr<T> uniq(new T(std::move(_value)));
        value_ptr_ = std::move(uniq);
        //value_ptr_ = std::move(std::unique_ptr<T>(_value));
        //value_ptr_.reset(_value);
        //value_ptr_ = std::move(_value);
    }
*/
private:
    std::unique_ptr<T> value_ptr_;
    Node* left_ptr_;
    Node* right_ptr_;
};

template <typename T>
class List {
public:
    class Iterator {
    public:
        Iterator& operator++();
        Iterator operator++(int);
        T& operator*() const;
        T* operator->() const;

        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;

        Iterator();
        Iterator(Node<T>* node);

    private:
        Node<T>* node_ptr_;
    };

    List();
    List(const List&);
    List(List&&);
    ~List();

    List& operator=(const List&);
    List& operator=(List&&);

    bool IsEmpty() const;
    size_t Size() const;

    void PushBack(const T& elem);
    void PushBack(T&& elem);
    void PushFront(const T& elem);
    void PushFront(T&& elem);

    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;

    void PopBack();
    void PopFront();

    Iterator Begin();
    Iterator End();

private:
    void clear_();

    Node<T>* end_;
    Node<T>* begin_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {
    return list.End();
}

template <typename T>
void List<T>::clear_() {
    for (Node<T>* current_ptr = begin_; current_ptr != end_;) {
       Node<T>* next = current_ptr->getRight();
       delete current_ptr;
       current_ptr = next;
    }
    begin_ = end_;
}

template <typename T>
List<T>::List() {
    end_ = new Node<T>();
    begin_ = end_;
}

template <typename T>
List<T>::List(const List<T>& other) {
    end_ = new Node<T>();
    begin_ = end_;
    for (Node<T>* current_other_ptr = other.end_;
         current_other_ptr != nullptr;
         current_other_ptr = current_other_ptr->getLeft()) {
        if (current_other_ptr == other.end_) {
            continue;
        }
        PushFront(current_other_ptr->getValue());
    }
}

template <typename T>
List<T>::List(List<T>&& other) {
    end_ = other.end_;
    begin_ = other.begin_;
    other.end_ = nullptr;
    other.begin_ = nullptr;
}

template <typename T>
List<T>::~List() {
    clear_();
    delete end_;
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    clear_();
    for (Node<T>* current_other_ptr = other.end_;
         current_other_ptr != nullptr;
         current_other_ptr = current_other_ptr->getLeft()) {
        if (current_other_ptr == other.end_) {
            continue;
        }
        PushFront(current_other_ptr->getValue());
    }
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other) {
    clear_();
    end_ = other.end_;
    begin_ = other.begin_;
    other.end_ = nullptr;
    other.begin_ = nullptr;
}

template <typename T>
bool List<T>::IsEmpty() const {
    return (Size() == 0);
}

template <typename T>
size_t List<T>::Size() const {
    size_t size = 0;
    for (Node<T>* current_ptr = begin_; current_ptr != end_; current_ptr = current_ptr->getRight()) {
        ++size;
    }
    return size;
}

template <typename T>
void List<T>::PushBack(const T& elem) {
    std::cerr << "this\n";
    Node<T>*node_ptr = new Node<T>();
    std::cerr << "heh\n";
    node_ptr->setValue(std::move(T(elem)));
    std::cerr << "kekoise\n";
    std::cerr << node_ptr->getValue() << "\n";
    node_ptr->setLeftPtr(end_->getLeft());
    node_ptr->setRightPtr(end_);
    if (end_->getLeft() != nullptr) {
        end_->getLeft()->setRightPtr(node_ptr);
    }
    end_->setLeftPtr(node_ptr);
    if (begin_ == end_) {
        begin_ = node_ptr;
    }
}

template <typename T>
void List<T>::PushBack(T&& elem) {
    std::cerr << "that\n";
    Node<T>* node_ptr = new Node<T>();
    node_ptr->setValue(std::move(elem));
    std::cerr << node_ptr->getValue() << "\n";
    node_ptr->setLeftPtr(end_->getLeft());
    node_ptr->setRightPtr(end_);
    if (end_->getLeft() != nullptr) {
        end_->getLeft()->setRightPtr(node_ptr);
    }
    end_->setLeftPtr(node_ptr);
    if (begin_ == end_) {
        begin_ = node_ptr;
    }
}

template <typename T>
void List<T>::PushFront(const T& elem) {
    Node<T>* node_ptr = new Node<T>();
    node_ptr->setValue(std::move(elem));
    node_ptr->setLeftPtr(nullptr);
    node_ptr->setRightPtr(begin_);
    begin_->setLeftPtr(node_ptr);
    begin_ = node_ptr;
}

template <typename T>
void List<T>::PushFront(T&& elem) {
    Node<T>* node_ptr = new Node<T>();
    node_ptr->setValue(std::move(elem));
    node_ptr->setLeftPtr(nullptr);
    node_ptr->setRightPtr(begin_);
    begin_->setLeftPtr(node_ptr);
    begin_ = node_ptr;
}

template <typename T>
T& List<T>::Front() {
    return begin_->getValue();
}

template <typename T>
const T& List<T>::Front() const {
    return begin_->getValue();
}

template <typename T>
T& List<T>::Back() {
    return end_->getLeft()->getValue();
}

template <typename T>
const T& List<T>::Back() const {
    return end_->getLeft()->getValue();
}

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    node_ptr_ = node_ptr_->getRight();
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator:: operator++(int pos) {
    List<T>::Iterator it;
    it.node_ptr_ = this->node_ptr_;
    for (int i = 0; i < pos; ++i) {
        it.node_ptr_ = it.node_ptr_->getRight();
    }
    return it;
}

template <typename T>
T& List<T>::Iterator::operator*() const {
    return this->node_ptr_->getValue();
}

template <typename T>
T* List<T>::Iterator::operator->() const {
    return &(this->node_ptr_->getValue());
}

template <typename T>
bool List<T>::Iterator::operator==(const List<T>::Iterator& rhs) const {
    return (this->node_ptr_ == rhs.node_ptr_);
}

template <typename T>
bool List<T>::Iterator::operator!=(const List<T>::Iterator& rhs) const {
    return (this->node_ptr_ != rhs.node_ptr_);
}

template <typename T>
List<T>::Iterator::Iterator(Node<T>* node){
    this->node_ptr_ = node;
}

template <typename T>
typename List<T>::Iterator List<T>::Begin() {
    List<T>::Iterator it (begin_);
    return it;
}

template <typename T>
typename List<T>::Iterator List<T>::End() {
    List<T>::Iterator it (end_);
    return it;
}
