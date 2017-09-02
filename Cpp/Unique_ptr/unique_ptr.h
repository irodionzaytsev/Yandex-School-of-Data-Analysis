#pragma once

class UniquePtr {
public:
    // put all required methods and operators here

    // see http://ru.cppreference.com/w/cpp/memory/unique_ptr
    // and test.cpp

    void reset() {
        if (ptr_ != nullptr) {
            delete ptr_;
        }
        ptr_ = nullptr;
    }

    void reset(TestClass* testClass) {
        reset();
        ptr_ = testClass;
    }

    UniquePtr() {
        ptr_ = nullptr;
    }

    UniquePtr(TestClass* testClass) {
        ptr_ = testClass;
    }



    TestClass* operator ->() const {
        return ptr_;
    }

    ~UniquePtr() {
        reset();
    }

    UniquePtr(UniquePtr&& p) {

        ptr_ = p.ptr_;
        p.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& p)
    {
        reset();
        ptr_ = p.ptr_;
        p.ptr_ = nullptr;
        return *this;
    }

private:
    TestClass* ptr_;

};
