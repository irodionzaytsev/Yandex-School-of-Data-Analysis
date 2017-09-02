#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <stdexcept>

class Matrix{
public:
    Matrix() {}

    Matrix (const std::vector<std::vector<int>> matrix) {
        this->matrix = matrix;
        first_side = matrix.size();
        for (const std::vector<int>& row : matrix) {
            if (second_side != -1 && second_side != row.size()) {
                throw std::length_error("constructor row length error");
            }
            second_side = row.size();
        }
    }

    void print() {
        for (int i = 0; i < first_side; ++i, std::cout << "\n") {
            for (int j = 0; j < second_side; ++j) {
                std::cout << matrix[i][j] << " ";
            }
        }
    }

    std::pair<int, int> size() {
        return std::make_pair(first_side, second_side);
    };

    int get_elem(int row, int col) {
        if (row < 0 ||
            row >= first_side ||
            col < 0 ||
            col >= second_side) {
            throw std::out_of_range("get_elem bad indexes");
        }
        return matrix[row][col];
    }

    Matrix operator + (const Matrix& other) {
        if (first_side != other.first_side ||
            second_side != other.second_side) {
            throw std::length_error("operator+ size error");
        }
        std::vector<std::vector<int>> elements = matrix;
        for (int i = 0; i < first_side; ++i) {
            for (int j = 0; j < second_side; ++j) {
                elements[i][j] += other.matrix[i][j];
            }
        }
        return Matrix(elements);
    }

private:
    std::vector<std::vector<int>> matrix;
    int first_side;
    int second_side = -1;
};
