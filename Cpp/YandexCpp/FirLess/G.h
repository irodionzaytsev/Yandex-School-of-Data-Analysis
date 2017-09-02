#include<iostream>

double binpow(double a, int n) {
    double sq = a;
    double ans = 1;
    while (n > 0) {
        if (n%2 == 1) {
            ans *= sq;
        }
        n = n/2;
        sq *= sq;
    }
    return ans;
}

int main() {
    double a;
    std::cin >> a;
    int n;
    std::cin >> n;
    std::cout << binpow(a, n) << std::endl;
}
