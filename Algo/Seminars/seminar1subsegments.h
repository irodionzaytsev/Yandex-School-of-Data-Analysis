#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

void readInput(std::vector<long long>* elements,
               long long* left,
               long long* right,
               std::istream &in_stream = std::cin) {
    int elements_count;
    in_stream >> elements_count;
    long long element;
    for (int i = 0; i < elements_count; ++i) {
        in_stream >> element;
        (*elements).push_back(element);
    }
    in_stream >> *left >> *right;
}

void writeOutput(long long result, std::ostream &out_stream = std::cout) {
    out_stream << result;
}

void printVector(const std::vector<long long>& some_vector) {
    for (size_t i = 0; i < some_vector.size(); ++i) {
        std::cerr << some_vector[i] << " ";
    }
    std::cerr << std::endl;
}

void printBoolVector (const std::vector<bool>& some_vector) {
    std::cerr << "true indexes are:\n";
    for (int i = 0; i < some_vector.size(); ++i) {
        if (some_vector[i]) {
            std::cerr << i << " ";
        }
    }
    std::cerr << std::endl;
}

void printPairVector(const std::vector<std::pair<int, int>>& some_vector) {
    for (size_t i = 0; i < some_vector.size(); ++i) {
        std::cerr << "{ " << some_vector[i].first << ", " << some_vector[i].second << "} ";
    }
    std::cerr << std::endl;
}

long long bruteForce(const std::vector<long long>& elements, long long least) {
    long long ans = 0;
    for (int i = 0; i < elements.size(); ++i) {
        long long subsum = 0;
        for (int j = i; j < elements.size(); ++j) {
            subsum += elements[j];
            if (subsum >= least) {
                ++ans;
            }
        }
    }
    return ans;
}

std::vector<long long> subSums(const std::vector<long long>& elements) {
    std::vector<long long> sub_sums;
    long long current = 0;
    for (int i = 0; i < elements.size(); ++i) {
        current += elements[i];
        sub_sums.push_back(current);
    }
    return sub_sums;
}

long long solve2Arrays(const std::vector<long long>& first_sub_sums,
                       const std::vector<long long>& second_sub_sums,
                       long long least) {
    //std::cerr << "first and second sub sums\n";
    //printVector(first_sub_sums);
    //printVector(second_sub_sums);
    if (first_sub_sums.size() == 0 || second_sub_sums.size() == 0) {
        return 0;
    }
    int left_pointer = first_sub_sums.size() - 1;
    int right_pointer = second_sub_sums.size() - 1;
    if (first_sub_sums[left_pointer] + second_sub_sums[right_pointer] < least) {
        return 0;
    }
    while(right_pointer >= 1 && first_sub_sums[left_pointer] + second_sub_sums[right_pointer - 1] >= least) {
        --right_pointer;
    }
    long long ans = 0;
    while (left_pointer >= 0) {
        while(right_pointer != second_sub_sums.size() &&
              first_sub_sums[left_pointer] + second_sub_sums[right_pointer] < least) {
            ++right_pointer;
        }
        if (right_pointer == second_sub_sums.size()) {
            return ans;
        } else {
            ans += (second_sub_sums.size() - right_pointer);
        }
        --left_pointer;
    }
    return ans;
}

long long solve(const std::vector<long long>& elements, long long least) {
    //printVector(elements);
    //std::cerr << "least=" << least << std::endl;
    if (elements.size() < 3) {
        return bruteForce(elements, least);
    }
    std::vector<long long> first_subvector;
    std::vector<long long> second_subvector;
    for (int i = 0; i < elements.size(); ++i) {
        if (i <= elements.size() / 2) {
            first_subvector.push_back(elements[i]);
        } else {
            second_subvector.push_back(elements[i]);
        }
    }
    long long ans = solve(first_subvector, least) + solve(second_subvector, least);
    //std::cerr << "ans = " << ans << std::endl;

    std::reverse(first_subvector.begin(), first_subvector.end());
    std::vector<long long> first_sub_sums = subSums(first_subvector);
    std::vector<long long> second_sub_sums = subSums(second_subvector);
    std::sort(first_sub_sums.begin(), first_sub_sums.end());
    std::sort(second_sub_sums.begin(), second_sub_sums.end());
    long long between_ans = solve2Arrays(first_sub_sums, second_sub_sums, least);
    //std::cerr << "between_ans = " << between_ans << std::endl;
    return ans + between_ans;
}

int main() {
    //int N = 50;
    //std::cout << N << std::endl;
    //for (int i = 0; i < N; ++i) {
    //    std::cout << rand() % N - N / 2 << " ";
    //}
    std::vector<long long> elements;
    long long left, right;
    readInput(&elements, &left, &right);
    long long right_result = solve(elements, right + 1);
    long long left_result = solve(elements, left);
    //std::cerr << "rigth_result= " << right_result << std::endl;
    //std::cerr << "left_result= " << left_result << std::endl;
    long long result = left_result - right_result;
    writeOutput(result);
    //std::cout << std::endl;
    //writeOutput(bruteForce(elements, left) - bruteForce(elements, right + 1));
}
