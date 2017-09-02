#include "CountStream.h"

int main() {

    CountingDict dict;

    for (int i = -10; i < 10; ++i) {
        dict.Insert(i);
    }

    for (int i = -10; i < 10; ++i) {
        std::cout << dict.GetCount(i) << "\n";
    }

    for (int i = 0; i < 56; ++i) {
        dict.Insert(-57);
    }

    std::cout << dict.GetCount(-57) << "\n";

    std::cout << dict.GetCount(228) << "\n";


}