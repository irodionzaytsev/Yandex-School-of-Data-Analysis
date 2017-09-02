#include <iostream>
#include <map>
#include <vector>

int main() {
    std::vector<int> list;
    std::map<int, int> mapa;
    int x;
    while( std::cin >> x ) {
        list.push_back(x);
        mapa[x]++;
        if (x == 0) {
            break;
        }
    }
    for (int i = 0; i < list.size(); ++i) {
        if (mapa[list[i]] == 1) {
            std::cout << list[i];
            mapa[list[i]]++;
        }
    }

    return 0;
}
