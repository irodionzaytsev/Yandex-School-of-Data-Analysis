#include <iostream>

#define ASSERT_EQ(expected, actual)                                                             \
    {                                                                                           \
        if (expected != actual) {                                                               \
            std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl;       \
            std::cerr << "\texpected: " << expected << " (= " << #expected << ")" << std::endl; \
            std::cerr << "\tgot: " << actual << " (= " << #actual << ")" << std::endl;          \
            std::terminate();                                                                   \
        }                                                                                       \
    }

struct Counters {
    Counters() { Reset(); }

    void Reset()
    {
        ctor_ = intctor_ = dtor_ = copyctor_ = movector_ = copyoperator_ = moveoperator_ = method_ = 0;
    }

    void Check(int ctor, int intctor, int dtor, int copyctor, int movector,
               int copyoperator, int moveoperator, int method)
    {
        ASSERT_EQ(ctor, ctor_);
        ASSERT_EQ(intctor, intctor_);
        ASSERT_EQ(dtor, dtor_);
        ASSERT_EQ(copyctor, copyctor_);
        ASSERT_EQ(movector, movector_);
        ASSERT_EQ(copyoperator, copyoperator_);
        ASSERT_EQ(moveoperator, moveoperator_);
        ASSERT_EQ(method, method_);
    }

    int ctor_;
    int intctor_;
    int dtor_;
    int copyctor_;
    int movector_;
    int copyoperator_;
    int moveoperator_;
    int method_;
};

Counters counters;

class TestClass {
public:
    TestClass() { ++counters.ctor_; }
    explicit TestClass(int) { ++counters.intctor_; }
    TestClass(const TestClass&) { ++counters.copyctor_; }
    TestClass(TestClass&&) { ++counters.movector_; }
    ~TestClass() { ++counters.dtor_; }

    TestClass& operator=(const TestClass&)
    {
        ++counters.copyoperator_;
        return *this;
    }
    TestClass& operator=(TestClass&&)
    {
        ++counters.moveoperator_;
        return *this;
    }

    void Method() { ++counters.method_; }
};

#include "unique_ptr.h"

void TestEmptyUniquePtr()
{
    counters.Reset();
    {
        UniquePtr ptr;
    }
    counters.Check(0, 0, 0, 0, 0, 0, 0, 0);
}

void TestSimpleUniquePtr()
{
    counters.Reset();
    {
        UniquePtr ptr(new TestClass);
        counters.Check(1, 0, 0, 0, 0, 0, 0, 0);
        ptr.reset();
        counters.Check(1, 0, 1, 0, 0, 0, 0, 0);
        ptr.reset(new TestClass(1));
        counters.Check(1, 1, 1, 0, 0, 0, 0, 0);
        ptr.reset(new TestClass(2));
        counters.Check(1, 2, 2, 0, 0, 0, 0, 0);
        ptr->Method();
        counters.Check(1, 2, 2, 0, 0, 0, 0, 1);
    }
    counters.Check(1, 2, 3, 0, 0, 0, 0, 1);
}

void TestMoveUniquePtr()
{
    counters.Reset();
    {
        UniquePtr ptr(new TestClass);
        UniquePtr ptr2;
        counters.Check(1, 0, 0, 0, 0, 0, 0, 0);
        ptr2 = std::move(ptr);
        counters.Check(1, 0, 0, 0, 0, 0, 0, 0);
        ptr2 = std::move(ptr);
        counters.Check(1, 0, 1, 0, 0, 0, 0, 0);
        ptr.reset();
        ptr2.reset();
    }
    counters.Check(1, 0, 1, 0, 0, 0, 0, 0);
    counters.Reset();
    {
        UniquePtr ptr(new TestClass);
        UniquePtr ptr2(new TestClass(2));
        counters.Check(1, 1, 0, 0, 0, 0, 0, 0);
        ptr = std::move(ptr2);
        counters.Check(1, 1, 1, 0, 0, 0, 0, 0);
        UniquePtr ptr3(std::move(ptr));
        counters.Check(1, 1, 1, 0, 0, 0, 0, 0);
    }
    counters.Check(1, 1, 2, 0, 0, 0, 0, 0);
}

int main()
{
    std::cout << "first\n";
    TestEmptyUniquePtr();
    //std::cerr << "second\n";
    TestSimpleUniquePtr();
    //std::cerr << "third\n";
    TestMoveUniquePtr();

    return 0;
}
