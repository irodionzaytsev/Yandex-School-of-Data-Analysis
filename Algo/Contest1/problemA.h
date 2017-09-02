#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

class VeryCoolMaxStack {
private:
    std::stack<std::pair<int, int>> inner_stack_;

public:
    VeryCoolMaxStack() {}

    int getSize() {
        return inner_stack_.size();
    }

    int getMax() {
        if (inner_stack_.size() > 0) {
            return inner_stack_.top().second;
        } else {
            return SMALLCONST;
        }
    }

    void push(const int element) {
        inner_stack_.push(std::make_pair(element, std::max(getMax(), element)));
    }

    int pop() {
        int top = inner_stack_.top().first;
        inner_stack_.pop();
        return top;
    }
};

class VeryCoolMaxQueue {
private:
    VeryCoolMaxStack first_inner_stack_;
    VeryCoolMaxStack second_inner_stack_;

public:
    void push(int element) {
        first_inner_stack_.push(element);
    }

    void move_elements() {
        while (first_inner_stack_.getSize() > 0) {
            second_inner_stack_.push(first_inner_stack_.pop());
        }
    }

    void pop() {
        if (second_inner_stack_.getSize() == 0) {
            move_elements();
        }
        second_inner_stack_.pop();
    }

    int getMax() {
        return std::max(first_inner_stack_.getMax(), second_inner_stack_.getMax());
    }
};

void readInput(std::vector<int>* elements,
               std::vector<char>* commands,
               std::istream &in_stream = std::cin) {
    int elements_number;
    in_stream >> elements_number;
    int element;
    for (int i = 0; i < elements_number; ++i) {
        in_stream >> element;
        (*elements).push_back(element);
    }
    int commands_number;
    in_stream >> commands_number;
    char c;
    for (int i = 0; i < commands_number; ++i) {
        in_stream >> c;
        (*commands).push_back(c);
    }
}

void writeOutput(std::vector<int>* maximas, std::ostream &out_stream = std::cout) {
    for (size_t i = 0; i < (*maximas).size(); ++i) {
        out_stream << (*maximas)[i] << " ";
    }
}

std::vector<int> solve(const std::vector<int>& elements, const std::vector<char>& commands) {
    VeryCoolMaxQueue veryCoolMaxQueue;
    veryCoolMaxQueue.push(elements[0]);
    int left_pointer = 0;
    int right_pointer = 0;
    std::vector<int> maximas;
    for (char cmd : commands) {
        if (cmd == 'R') {
            ++right_pointer;
            veryCoolMaxQueue.push(elements[right_pointer]);
        }
        if (cmd == 'L') {
            ++left_pointer;
            veryCoolMaxQueue.pop();
        }
        maximas.push_back(veryCoolMaxQueue.getMax());
    }
    return maximas;
}

int main() {
    std::vector<int> elements;
    std::vector<char> commands;
    readInput(&elements, &commands);
    std::vector<int> maximas = solve(elements, commands);
    writeOutput(&maximas);
}
