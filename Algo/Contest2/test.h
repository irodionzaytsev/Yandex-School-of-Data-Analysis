#include <iostream>
#include <vector>
#include <algorithm>
#include <typeinfo>

struct Player{
    long long effectiveness_;
    int index_;
};
/*
Player::Player(long long _effectiveness, int _index) :
        effectiveness_(_effectiveness),
        index_(_index) {};

long long Player::getEffectiveness() const {
    return effectiveness_;

}

int Player::getIndex() const {
    return index_;
}
*/

int main() {

    std::vector<Player> v;
    for (int i = 0; i < 20; ++i) {
        v.push_back(Player{rand() % 10, rand() % 10});
    }

    for (auto& x : v) {
        std::cerr << x.effectiveness_<< " : " << x.index_ << "\n";
    }
    std::cerr << "\n";


    return 0;
}


