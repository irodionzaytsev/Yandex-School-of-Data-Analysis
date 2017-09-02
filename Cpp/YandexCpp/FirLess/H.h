#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
#include<utility>

void print(const std::vector<std::vector<char>>& v) {
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v[i].size(); ++j) {
            std::cout << v[i][j];
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string s;
    std::string tmp;
    while (std::cin >> tmp) {
        s += tmp;
    }
    std::map<char, int> mapa;
    for (int i = 0; i < s.length(); ++i) {
        mapa[s[i]]++;
    }
    std::vector<std::pair<char, int>> v;
    for (auto it = mapa.begin(); it != mapa.end(); ++it) {
        v.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(v.begin(), v.end());
    int maxx = -1;
    for (int i = 0; i < v.size(); ++i) {
        maxx = std::max(maxx, v[i].second);
    }
    int n = v.size();
    std::vector<std::vector<char>> grid(maxx + 1, std::vector<char>(n, '#'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < maxx; ++j) {
            if (j < maxx - v[i].second) {
                grid[j][i] = ' ';
            }
        }
        grid[maxx][i] = v[i].first;
    }
    print(grid);
    return 0;
}
