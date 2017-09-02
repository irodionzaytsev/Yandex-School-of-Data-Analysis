#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <iomanip>

const int SMALLCONST = -1000000001;

void printVector(const std::vector<int>& some_vector) {
    for (size_t i = 0; i < some_vector.size(); ++i) {
        std::cerr << some_vector[i] << " ";
    }
    std::cerr << std::endl;
}

template<typename T>
void printVectorVector(const std::vector<std::vector<T>>& some_vector) {
    std::cerr << "\n";
    for (auto& sub_vector : some_vector) {
        for (auto &element : sub_vector) {
            element.print();
        }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}

template<typename T>
void printVector(const std::vector<T>& some_vector) {
    std::cerr << "\n";
    for (auto &element : some_vector) {
        element.print();
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

template<typename T>
class Heap {

public:
    int size_;
    std::vector<T> elements_;

    bool heapify(int index) {
        if (2 * index + 1 >= size_) {
            return false;
        }
        if (2 * index + 2 >= size_) {
            if (elements_[index] > elements_[2 * index + 1]) {
                std::swap(elements_[index], elements_[2 * index + 1]);
                return true;
            }
            return false;
        }
        if (elements_[index] <= elements_[2 * index+1] &&
            elements_[index] <= elements_[2 * index + 2]) {
            return false;
        }
        if (elements_[2 * index + 1] < elements_[2 * index + 2]) {
            std::swap(elements_[index], elements_[2 * index + 1]);
            heapify(2 * index + 1);
            return true;
        } else {
            std::swap(elements_[index], elements_[2 * index + 2]);
            heapify(2 * index + 2);
            return true;
        }
    }


    Heap() {
        size_ = 0;
    }

    void add(T element) {
        ++size_;
        elements_.push_back(element);
        if (size_ == 1) {
            return;
        }
        bool flag = true;
        int current_index = (size_ - 2) / 2;
        while (flag) {
            flag = heapify(current_index);
            current_index = (current_index - 1) / 2;
        }
    }

    T getMin() {
        return elements_[0];
    }

    void pop() {
        std::swap(elements_[0], elements_[size_ - 1]);
        elements_.pop_back();
        --size_;
        heapify(0);
    }

    int getSize() {
        return size_;
    }

    bool isEmpty() {
        return elements_.empty();
    }
};

template<typename T>
void heap_sort(std::vector<T>* elements) {
    Heap<T> heap;
    while (!(*elements).empty()) {
        heap.add((*elements).back());
        (*elements).pop_back();
    }
    while (!heap.isEmpty()) {
        (*elements).push_back(heap.getMin());
        heap.pop();
    }
}

class FiniteHeap{

private:
    Heap<int> heap_;
    std::vector<bool> delete_queue_;

public:
    explicit FiniteHeap(const int reserve) {
        delete_queue_.assign(reserve, false);
    }

    void add(const int element) {
        heap_.add(-element);
    }

    int getMax() {
        while (delete_queue_[-heap_.getMin()]) {
            delete_queue_[-heap_.getMin()] = false;
            heap_.pop();
        }
        return -heap_.getMin();
    }

    void pop() {
        heap_.pop();
    }

    void erase_element(int element) {
        delete_queue_[element] = true;
    }
};

struct Point{

    int coord;
    int index;
    bool open;
    int color;

    Point(const int _coord, const int _index, const int _open, const int _color) :
            coord (_coord),
            index (_index),
            open (_open),
            color (_color) {};

    bool operator < (const Point& other) const {
        return this->coord < other.coord;
    }

    bool operator <= (const Point& other) const {
        return this->coord <= other.coord;
    }

    bool operator > (const Point& other) const {
        return this->coord > other.coord;
    }

    bool operator >= (const Point& other) const {
        return this->coord >= other.coord;
    }

    void print() const {
        std::cerr << coord << " " << index << " " << open << " " << color << "; ";
    }
};

struct Rectangle {

    int left_x;
    int left_y;
    int right_x;
    int right_y;
    int color;

    Rectangle() {};
};

struct Segment {
    int left;
    int right;
    int color;

    Segment(int _left, int _right, int _color) :
            left(_left),
            right(_right),
            color(_color) {};

    void print() const {
        std::cerr << left << " " << right << " " << color << "; ";
    }
};

std::vector<int> solve(const int len, const std::vector<Segment>& segments, const int colors) {
    std::vector<int> ans(colors + 1, 0);
    FiniteHeap finiteHeap(segments.size());
    std::vector<Point> segment_points;
    int index = -1;
    for (const auto& segment : segments) {
        ++index;
        segment_points.push_back(Point(segment.left, index, true, segment.color));
        segment_points.push_back(Point(segment.right, index, false, segment.color));
    }
    std::vector<Point> segment_points_copy = segment_points;
    std::sort(segment_points.begin(), segment_points.end());
    int current_color = 1;
    int prev_coord = 0;
    for (auto& point : segment_points) {
        int current_coord = point.coord;
        ans[current_color] += (current_coord - prev_coord);
        prev_coord = current_coord;
        if (point.open) {
            finiteHeap.add(point.index);
        } else {
            finiteHeap.erase_element(point.index);
        }
        current_color = segment_points_copy[2 * finiteHeap.getMax()].color;
    }
    return ans;
}

std::vector<std::pair<int, int>> solve(const int width,
                                       const int height,
                                       const int count,
                                       const std::vector<Rectangle>& rectangles) {
    std::vector<std::vector<Segment>> segments(width);
    std::vector<bool> possible_x (width + 1, false);

    for (auto& rectangle : rectangles) {
        possible_x[rectangle.left_x] = true;
        possible_x[rectangle.right_x] = true;
    }

    std::vector<int> possible_xses;
    for (int i = 0; i <= width; ++i) {
        if (possible_x[i]) {
            possible_xses.push_back(i);
        }
    }

    for (int i = 0; i < count; ++i) {
        Segment segment(rectangles[i].left_y, rectangles[i].right_y, rectangles[i].color);
        for (int current_x : possible_xses) {
            if (current_x < rectangles[i].left_x || current_x >= rectangles[i].right_x) {
                continue;
            }
            segments[current_x].push_back(segment);
        }
    }

    int max_color = -1;
    for (int i = 0; i < count; ++i) {
        max_color = std::max(max_color, rectangles[i].color);
    }

    std::vector<int> answers(max_color + 1, 0);

    for (int i = 0; i + 1 < possible_xses.size(); ++i) {

        int current_x = possible_xses[i];
        int next_x = possible_xses[i + 1];
        std::vector<int> sub_answer = solve(height, segments[current_x], max_color);
        for (int j = 0; j <= max_color; ++j) {
            answers[j] += (next_x - current_x) * sub_answer[j];
        }
    }

    std::vector<std::pair<int, int>> result;
    for (int i = 0; i <= max_color; ++i) {
        if (answers[i] > 0) {
            result.push_back(std::make_pair(i, answers[i]));
        }
    }

    return result;
}


std::vector<std::pair<int, int>> old_solve(const int width,
                                       const int height,
                                       const int count,
                                       const std::vector<Rectangle>& rectangles) {

    std::vector<std::vector<Segment>> segments(width);

    for (int i = 0; i < count; ++i) {
        Segment segment(rectangles[i].left_y, rectangles[i].right_y, rectangles[i].color);
        for (int j = rectangles[i].left_x; j < rectangles[i].right_x; ++j) {
            segments[j].push_back(segment);
        }
    }

    int max_color = -1;
    for (int i = 0; i < count; ++i) {
        max_color = std::max(max_color, rectangles[i].color);
    }

    std::vector<int> answers(max_color + 1, 0);

    for (int i = 0; i < width; ++i) {
        std::vector<int> sub_answer = solve(height, segments[i], max_color);
        for (int j = 0; j <= max_color; ++j) {
            answers[j] += sub_answer[j];
        }
    }

    std::vector<std::pair<int, int>> result;
    for (int i = 0; i <= max_color; ++i) {
        if (answers[i] > 0) {
            result.push_back(std::make_pair(i, answers[i]));
        }
    }

    return result;
}

void readInput(int* width,
               int* height,
               int* count,
               std::vector<Rectangle>* rectangles,
               std::istream &in_stream = std::cin) {
    in_stream >> *width;
    in_stream >> *height;
    in_stream >> *count;
    int left_x;
    int left_y;
    int right_x;
    int right_y;
    int color;
    (*rectangles).resize(*count + 1);
    (*rectangles)[0].left_x = 0;
    (*rectangles)[0].left_y = 0;
    (*rectangles)[0].right_x = *width;
    (*rectangles)[0].right_y = *height;
    (*rectangles)[0].color = 1;
    for (int i = 1; i <= *count; ++i) {
        in_stream >> left_x >> left_y >> right_x >> right_y >> color;
        (*rectangles)[i].left_x = left_x;
        (*rectangles)[i].left_y = left_y;
        (*rectangles)[i].right_x = right_x;
        (*rectangles)[i].right_y = right_y;
        (*rectangles)[i].color = color;
    }
}

void writeOutput(const std::vector<std::pair<int, int>>& result,
                 std::ostream &out_stream = std::cout) {
    for (auto& color_occurings : result) {
        out_stream << color_occurings.first << " " << color_occurings.second << "\n";
    }
    out_stream << "\n";
}

int main() {

    /*
    int w = 400, h = 400, rects = 220;
    int colers = 20;
    std::cerr << w << " " << h << " " << rects << "\n";
    for (int i = 0; i < rects; ++i) {
        int a1 = rand() % w, a2 = rand() % w;
        int b1 = rand() % h, b2 = rand() % h;
        int color = rand() % colers + 1;
        std::cerr << std::min(a1, a2) << " "
                  << std::min(b1, b2) << " "
                  << std::max(a1, a2) << " "
                  << std::max(b1, b2) << " "
                  << color << "\n";
    }
     */

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int width, height, count;
    std::vector<Rectangle> rectangles;
    readInput(&width, &height, &count, &rectangles);
    std::vector<std::pair<int, int>> result = solve(width, height, count + 1, rectangles);

    writeOutput(result);
}
