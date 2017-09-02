#include<iostream>
#include<cmath>

int main() {
    double t1, t2, h1, h2;
    std::cin >> h1 >> t1 >> h2 >> t2;
    double dif_t = t1 - t2;
    double rel_t = sqrt(h1/h2);
    double tt2 = dif_t / (rel_t - 1);
    std::cout << t2 - tt2;
}
