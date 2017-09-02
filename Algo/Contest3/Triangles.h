#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>

class Triangle {

private:
    int a_side;
    int b_side;
    int c_side;
    int label_;

    int gcd_ (int first, int second) {
        while (second) {
            first %= second;
            std::swap(first, second);
        }
        return first;
    }

public:
    void normalize() {
        if (a_side > b_side) {
            std::swap(a_side, b_side);
        }
        if (a_side > c_side) {
            std::swap(a_side, c_side);
        }
        if (b_side > c_side) {
            std::swap(b_side, c_side);
        }

        int gcd = gcd_(a_side, gcd_(b_side, c_side));
        a_side /= gcd;
        b_side /= gcd;
        c_side /= gcd;
    }

    Triangle(int _a_side, int _b_side, int _c_side) :
        a_side (_a_side),
        b_side (_b_side),
        c_side (_c_side) {
        label_ = 0;
    }

    int getHash(int prime) {
        return (a_side % prime +
                (b_side % prime) * prime +
                (c_side % prime) * prime * prime);
    }

    void setLabel(int label) {
        this->label_ = label;
    }

    int getLabel() {
        return this->label_;
    }

    friend std::ostream& operator<< (std::ostream& os, const Triangle& obj){
        os << obj.a_side << " " << obj.b_side <<  " " << obj.c_side << " : " << obj.label_ << "\n";
        return os;
    }
};

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
}

void readInput(size_t* triangles_count,
               std::vector<Triangle*>* triangles,
               std::istream &in_stream = std::cin) {
    int a_side;
    int b_side;
    int c_side;
    in_stream >> *triangles_count;
    for (int i = 0; i < *triangles_count; ++i) {
        in_stream >> a_side >> b_side >> c_side;
        triangles->push_back(new Triangle(a_side, b_side, c_side));
        triangles->back()->normalize();
    }
}


void makeRandom(size_t* triangles_count,
               std::vector<Triangle*>* triangles,
               std::istream &in_stream = std::cin) {
    int a_side;
    int b_side;
    int c_side;
    *triangles_count = 1000000;
    for (int i = 0; i < *triangles_count; ++i) {
        a_side = rand() % 1000000000 + 1;
        b_side = rand() % 1000000000 + 1;
        c_side = rand() % 1000000000 + 1;
        triangles->push_back(new Triangle(a_side, b_side, c_side));
        triangles->back()->normalize();
    }
}

void writeOutput(int result, std::ostream &out_stream = std::cout) {
    out_stream << result << "\n";
}

void reLabel(std::vector<Triangle*>* triangles, const int prime) {
    // printPointers(*triangles);
    std::vector<std::vector<Triangle*>> hashmap(prime * prime *prime);
    for (Triangle* triangle : *triangles) {
        hashmap[triangle->getHash(prime)].push_back(triangle);
    }
    int label = -1;
    std::vector<int> new_labels(prime * prime * prime, -1);

    for (auto& component : hashmap) {
        std::list<int> used;
        for (Triangle* triangle : component) {
            if (new_labels[triangle->getLabel()] == -1) {
                used.push_back(triangle->getLabel());
                ++label;
                new_labels[triangle->getLabel()] = label;
            }
            triangle->setLabel(new_labels[triangle->getLabel()]);
        }
        for (int used_label : used) {
            new_labels[used_label] = -1;
        }
    }
}

int solve(const size_t players_count, std::vector<Triangle*>* triangles) {

    // printPointers(*triangles);

    std::vector<int> primes({101, 103, 107});

    clock_t begin = clock();

    for (int prime : primes) {
        clock_t begin = clock();
        reLabel(triangles, prime);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cerr << "elapsed_secs = " << elapsed_secs << "\n";
    }

    int maxLabel = 0;

    for (Triangle* triangle : *triangles) {
        maxLabel = std::max(maxLabel, triangle->getLabel());
    }

    return maxLabel + 1;
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t triangles_count;
    std::vector<Triangle*> triangles;
    makeRandom(&triangles_count, &triangles);
    // readInput(&triangles_count, &triangles);
    int result = solve(triangles_count, &triangles);
    for (auto it : triangles) {
        delete it;
    }
    writeOutput(result);
}
