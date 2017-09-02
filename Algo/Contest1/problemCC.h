#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <climits>
#include <string>
#include <algorithm>

using std::vector;
using std::max_element;

inline int nextInt() {
    int number;
    std::cin >> number;
    return number;
}

vector<int> readData() {
    const int countOfNumbers = nextInt();
    vector<int> data(countOfNumbers);
    for (int i = 0; i < countOfNumbers; ++i) {
        data[i] = nextInt();
    }
    return data;
}

vector<int> longestAlternateChain(const vector<int>& data) {
    const int countOfNumbers = data.size();
    vector<int> longestAlternateChainWithBigStart(countOfNumbers, 1);
    vector<int> longestAlternateChainWithSmallStart(countOfNumbers, 1);
    vector<int> nextElementIfBigStart(countOfNumbers, countOfNumbers);
    vector<int> nextElementIfSmallStart(countOfNumbers, countOfNumbers);
    for (int index = countOfNumbers - 1; index >= 0; --index) {
        for (int next = index + 1; next < countOfNumbers; ++next) {
            if (data[next] < data[index]) {
                if (longestAlternateChainWithBigStart[index] <
                    longestAlternateChainWithSmallStart[next] + 1) {
                    longestAlternateChainWithBigStart[index] =
                            longestAlternateChainWithSmallStart[next] + 1;
                    nextElementIfBigStart[index] = next;
                }
            }
            if (data[next] > data[index]) {
                if (longestAlternateChainWithSmallStart[index] <
                    longestAlternateChainWithBigStart[next] + 1) {
                    longestAlternateChainWithSmallStart[index] =
                            longestAlternateChainWithBigStart[next] + 1;
                    nextElementIfSmallStart[index] = next;
                }
            }
        }
    }

    int maxIndexWithBigStart = std::max_element(
            longestAlternateChainWithBigStart.begin(),
            longestAlternateChainWithBigStart.end()) -
                               longestAlternateChainWithBigStart.begin();
    int maxIndexWithSmallStart = std::max_element(
            longestAlternateChainWithSmallStart.begin(),
            longestAlternateChainWithSmallStart.end()) -
                                 longestAlternateChainWithSmallStart.begin();

    int index = std::min(maxIndexWithBigStart, maxIndexWithSmallStart);
    bool isBigStart = false;
    if (longestAlternateChainWithSmallStart[maxIndexWithSmallStart] >
        longestAlternateChainWithBigStart[maxIndexWithBigStart]) {
        index = maxIndexWithSmallStart;
    } else if (longestAlternateChainWithBigStart[maxIndexWithBigStart] >
               longestAlternateChainWithSmallStart[maxIndexWithSmallStart]) {
        index = maxIndexWithBigStart;
        isBigStart = true;
    } else if (maxIndexWithSmallStart < maxIndexWithBigStart) {
        index = maxIndexWithSmallStart;
    } else if (maxIndexWithSmallStart > maxIndexWithBigStart) {
        index = maxIndexWithBigStart;
        isBigStart = true;
    } else {
        if (nextElementIfSmallStart[maxIndexWithSmallStart] <
            nextElementIfBigStart[maxIndexWithBigStart]) {
            isBigStart = false;
        }
    }

    vector<int> result;
    while (index < countOfNumbers) {
        result.push_back(data[index]);

        if (isBigStart) {
            index = nextElementIfBigStart[index];
        } else {
            index = nextElementIfSmallStart[index];
        }
        isBigStart ^= true;
    }

    return result;
}

int main() {
    std::cin.sync_with_stdio(0);
    std::cout.sync_with_stdio(0);

    vector<int> data = readData();

    vector<int> longestAlternateSequence = longestAlternateChain(data);
    for (int element : longestAlternateSequence) {
        std::cout << element << " ";
    }

    std::cout << "\n";

    return 0;
}
