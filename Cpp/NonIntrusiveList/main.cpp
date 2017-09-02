#include "intrusive_list.h"
#include <iostream>

struct Item : public ListHook {
public:
    int kek;

    Item(int _kek) {kek = _kek;}
    Item(const Item&) = delete;
    Item(Item&&) = delete;
    Item& operator = (const Item&) = delete;
    Item& operator = (Item&&) = delete;

    ~Item() {
        std::cerr << "I was deleted " << kek << "\n";
    }

    friend std::ostream& operator<< (std::ostream& os, const Item& obj) {
        os << obj.kek << " ";
        return os;
    }

};


void Example() {
    Item i0(0);
    List<Item> l;
    Item i1(1), i2(2), i3(3);

    l.PushBack(&i0);
    l.PushBack(&i1);
    l.PushBack(&i2);
    l.PushBack(&i3);

    //l.PushBack(new Item(4));
    //l.PushBack(new Item(5));

    for (auto it = l.Begin(); it != l.End(); ++it) {
        std::cerr << *it << " ";
    }
    std::cerr << "\n";
    // l.IteratorTo(&i1)->Unlink();
    for (auto it = l.Begin(); it != l.End(); ++it) {
        std::cerr << *it << " ";
    }
    std::cerr << "\n";

    std::cerr << (++l.IteratorTo(&i2) == l.IteratorTo(&i3)) << "\n";

    std::cerr << l.Back() << "\n";

    //l.IteratorTo(&i3)->Unlink();
    l.Back().Unlink();
    for (auto it = l.Begin(); it != l.End(); ++it) {
        std::cerr << *it << " ";
    }
    std::cerr << "\n";
    std::cerr << l.Back() << "\n";

    std::cerr << (&l.Back() == &i3) << "\n";

    std::cerr << l.IsEmpty() << "\n";
    l.Back().Unlink();
    std::cerr << l.IsEmpty() << "\n";
    l.Front().Unlink();
    std::cerr << l.IsEmpty() << "\n";
    l.Back().Unlink();
    std::cerr << l.IsEmpty() << "\n";
    l.PushFront(new Item(5));
    std::cerr << l.IsEmpty() << "\n";
    l.PopFront();
    std::cerr << l.IsEmpty() << "\n";

    l.PushBack(&i0);
    l.PushBack(&i1);
    l.PushBack(&i2);
    l.PushBack(&i3);

    for (auto it = l.Begin(); it != l.End(); ++it) {
        std::cerr << *it << " ";
    }
    std::cerr << "\n";

    std::cerr << (l.IteratorTo(&i2) == l.IteratorTo(&i2)) << "\n";
    std::cerr << (++l.IteratorTo(&i1) == l.IteratorTo(&i2)) << "\n";

    l.IteratorTo(&i2)->Unlink();

    for (auto it = l.Begin(); it != l.End(); ++it) {
        std::cerr << *it << " ";
    }
    std::cerr << "\n";

    List<Item> l_empty;
    std::cerr << "empty size " << l_empty.Size() << "\n";
    std::cerr << "empty isEmpty() " << l_empty.IsEmpty() << "\n";

    List<Item> l_moved = std::move(l);
    std::cerr << "have moved\n";
    for (auto it = l_moved.Begin(); it != l_moved.End(); ++it) {
        std::cerr << *it << " ";
    }
    std::cerr << "\n";

    std::cerr << "l size = " << l.Size() << "\n";
    std::cerr << "l empty = " << l.IsEmpty() << "\n";

    std::cerr << (l_moved.IteratorTo(&i3)->IsLinked()) << "\n";
    std::cerr << &i3 << "\n";

}

int main() {
    Example();
}
