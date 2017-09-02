#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

void readInput(std::string* bracket_sequence,
               std::istream &in_stream = std::cin) {
    in_stream >> *bracket_sequence;
}

void writeOutput(int result, std::ostream &out_stream = std::cout) {
    if (result == -1) {
        out_stream << "CORRECT";
    } else {
        out_stream << result;
    }
}

bool is_close(char bracket) {
    return (bracket == ')' || bracket == ']' || bracket == '}');
}

bool match(char first_bracket, char second_bracket) {
    return (
            (first_bracket == '(' && second_bracket == ')') ||
            (first_bracket == '[' && second_bracket == ']') ||
            (first_bracket == '{' && second_bracket == '}')
    );
}

bool add_bracket(std::stack<char>* bracket_stack, char bracket) {
    if (!is_close(bracket)) {
        (bracket_stack->push(bracket));
        return true;
    }
    if (bracket_stack->size() == 0) {
        return false;
    }
    if (!match(bracket_stack->top(), bracket)) {
        return false;
    } else {
        bracket_stack->pop();
        return true;
    }
}

int solve(const std::string& bracket_sequence) {
    std::stack<char> bracket_stack;
    for (size_t i = 0; i < bracket_sequence.size(); ++i) {
        if (!add_bracket(&bracket_stack, bracket_sequence[i])) {
            return i;
        }
    }
    if (bracket_stack.size() > 0) {
        return bracket_sequence.size();
    } else {
        return -1;
    }
}

int main() {
    std::string bracket_sequence;
    readInput(&bracket_sequence);
    int result = solve(bracket_sequence);
    writeOutput(result);
}
