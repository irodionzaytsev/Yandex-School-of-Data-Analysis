#include <list>
#include <iostream>
#include <memory>
#include "list2.h"

class TestClass {
private:
    int kek;
public:
    TestClass() = delete;
    explicit TestClass(int t) {
        kek = t;
    }
    TestClass(const TestClass&) = delete;
    TestClass(TestClass&&) = default;
    ~TestClass() = default;

    TestClass& operator=(const TestClass&) = delete;
    TestClass& operator=(TestClass&&) = default;

    void Method() { ++kek;}

    friend std::ostream& operator<<(std::ostream& os, const TestClass& tc) {
        os << tc.kek;
        return os;
    }
};

int main() {
/*
    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    std::list<int> l2 = l;
    l.pop_back();

    for (auto x : l) {
        std::cerr << x <<  " ";
    }
    std::cerr << "\n";

    for (auto x : l2) {
        std::cerr << x <<  " ";
    }
    std::cerr << "\n";


    List<int> listez;
    std::cerr << "init\n";
    listez.PushBack(3);
    std::cerr << "pushback 3\n";
    listez.PushBack(5);
    std::cerr << "pushback 5\n";
    std::cerr << listez.Back() << "\n";
    std::cerr << listez.Front() << "\n";
    listez.PushFront(1);
    listez.PushFront(4);
    for (auto x : listez) {
        std::cerr << x << " ";
    }
    std::cerr << "\n";

    List<int> listez2 = listez;

    for (auto x : listez2) {
        std::cerr << x << " ";
    }
    std::cerr << "\n";

    List<int> listez3 = std::move(listez2);

    for (auto x : listez2) {
        std::cerr << x << " ";
    }
    std::cerr << "\n";

    for (auto x : listez3) {
        std::cerr << x << " ";
    }
    std::cerr << "\n";

    List<TestClass> test_list;
    TestClass tc(3);
    test_list.PushBack(tc);


    for (auto&& x : test_list) {
        std::cerr << x << "\n";
    }
*/
/*
    std::list<std::unique_ptr<TestClass>> listik;
    TestClass tc3(3);
    TestClass tc5(5);
    listik.push_back(tc3);
    listik.push_back(tc5);

    for (auto&& x : listik) {
        std::cerr << x << " ";
    }
*/

    List<int> listock;

    listock.PushBack(3);
    std::cerr << "hm\n";
    listock.PushBack(5);


    for (auto x : listock) {
        std::cerr << x << "\n";
    }

    List<TestClass> listez;

    TestClass tc3(33);
    TestClass tc5(55);
    TestClass tc7(77);
    listez.PushFront(std::move(tc3));
    listez.PushFront(std::move(tc5));
    listez.PushFront(std::move(tc7));

    for (auto&& x : listez) {
        std::cerr << x << "\n";
    }
    std::cerr << "end\n";

    listez.PopBack();

    for (auto&& x : listez) {
        std::cerr << x << "\n";
    }
    std::cerr << "end\n";

    listez.PopFront();

    for (auto&& x : listez) {
        std::cerr << x << "\n";
    }
    std::cerr << "end\n";


}