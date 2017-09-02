#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>
#include <sstream>

template <typename T>
void printPointers(const std::vector<T*>& some_vector) {
    for (T* element : some_vector) {
        std::cerr << *element << "\n";
    }
}

template <typename T>
void printVector(const std::vector<T>& some_vector) {
    for (T element : some_vector) {
        std::cerr << element << "\n";
    }
    std::cerr << "\n";
}

template <typename T>
void printVector(const std::vector<std::vector<T>>& some_vector) {
    std::cerr << "vector vector:\n";
    for (auto sub_vector : some_vector) {
        std::cerr << ": ";
        for (T& element : sub_vector) {
            std::cerr << element << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";
}

void readInput(int* worker_count,
               int* request_count,
               std::vector<std::vector<int>>* requests,
               std::istream &in_stream = std::cin) {
    in_stream >> *worker_count;
    in_stream >> *request_count;
    std::string str;
    std::getline(in_stream, str);
    for (int index = 0; index < *request_count; ++index) {
        std::string str;
        std::getline(in_stream, str);
        std::stringstream ss;
        ss << str;
        std::vector<int> elements;
        int element;
        while (ss >> element) {
            elements.push_back(element);
        }
        // for (int elem : elements) {
        //     std::cerr << elem << " ";
        // }
        // std::cerr << "\n";
        requests->push_back(elements);
    }
}

void writeOutput(const std::vector<int> result,
                 std::ostream &out_stream = std::cout) {
    for (int element : result) {
        std::cout << element << "\n";
    }
}

struct Worker {

    explicit Worker(int _index) {
        index = _index;
        parent = _index;
    }

    int index;
    int parent;
};

int getDirector(int index, std::vector<Worker>* workers) {
    if ((*workers)[index].parent == index) {
        return index;
    } else {
        int director = getDirector((*workers)[index].parent, workers);
        (*workers)[index].parent = director;
        return director;
    }
}

std::vector<int> solve(const int workers_count,
                       const int requests_count,
                       const std::vector<std::vector<int>>& requests) {
    std::vector<int> result;
    std::vector<Worker> workers;
    for (int i = 0; i < workers_count; ++i) {
        workers.push_back(Worker(i));
    }
    for (int request = 0; request < requests.size(); ++request) {
        if (requests[request].size() == 1) {
            int index = requests[request][0] - 1;
            // std::cerr << "index = " << index << "\n";
            int director = getDirector(index, &workers);
            // std::cerr << "director = " << director << "\n";
            result.push_back(director + 1);
        } else {
            int parent = requests[request][0] - 1;
            int index = requests[request][1] - 1;
            if (getDirector(index, &workers) == index && getDirector(parent, &workers) != index) {
                workers[index].parent = parent;
                result.push_back(1);
                // std::cerr << "result = 1\n";
            } else {
                result.push_back(0);
                // std::cerr << "result = 0\n";
            }
            // std::cerr << "index = " << index << ", parent = " << parent << "\n";
        }
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int workers_count;
    int requests_count;
    std::vector<std::vector<int>> requests;
    readInput(&workers_count, &requests_count, &requests);
    std::vector<int> answer = solve(workers_count, requests_count, requests);
    writeOutput(answer);
    return 0;
}
