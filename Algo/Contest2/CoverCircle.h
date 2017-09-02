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

struct MarkedPoint {

    long double coord_x_;
    int type_;

    explicit MarkedPoint(long double coord_x, int type) :
            coord_x_(coord_x),
            type_(type) {};

    bool operator < (MarkedPoint& other) {
        return this->coord_x_ < other.coord_x_;
    }
};

class Point {
private:
    int coord_x_;
    int coord_y_;

public:
    explicit Point(int coord_x, int coord_y) :
            coord_x_(coord_x),
            coord_y_(coord_y) {};

    std::vector<MarkedPoint> getSegment(long double radii) const {
        std::vector<MarkedPoint> markedPoints;
        long double disp = (radii - coord_y_) * (radii + coord_y_);
        if (disp <= 0.0) {
            return markedPoints;
        }
        long double disperse = sqrt(disp);
        markedPoints.push_back(MarkedPoint(coord_x_ - disperse, 1));
        markedPoints.push_back(MarkedPoint(coord_x_ + disperse, -1));
        return markedPoints;
    }
};

void readInput(int* points_count,
               int* cover,
               std::vector<Point>* points,
               std::istream &in_stream = std::cin) {
    in_stream >> *points_count;
    in_stream >> *cover;
    int coord_x;
    int coord_y;
    for (int i = 0; i < *points_count; ++i) {
        in_stream >> coord_x >> coord_y;
        (*points).push_back(Point(coord_x, coord_y));
    }
}

void writeOutput(long double result, std::ostream &out_stream = std::cout) {
    out_stream << std::setprecision(6);
    out_stream << std::fixed;
    out_stream << result;
}

bool check(const std::vector<Point> points, int cover, long double radii) {
    std::vector<MarkedPoint> markedPoints;
    for (const Point& point : points) {
        std::vector<MarkedPoint> segment = point.getSegment(radii);
        for (const MarkedPoint& marked_point : segment) {
            markedPoints.push_back(marked_point);
        }
    }

    std::sort(markedPoints.begin(), markedPoints.end());

    int counter = 0;
    for (const MarkedPoint& point : markedPoints) {
        counter += point.type_;
        if (counter >= cover) {
            return true;
        }
    }
    return false;
}

long double solve(int points_count,
                  int cover,
                  std::vector<Point> points) {
    long double min_radii = 0.0;
    long double max_radii = 1500.0;
    while (max_radii - min_radii > 0.00000001) {
        long double mid_radii = (min_radii + max_radii) / 2.0;
        if (check(points, cover, mid_radii)) {
            max_radii = mid_radii;
        } else {
            min_radii = mid_radii;
        }
    }
    return max_radii;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int points_count, cover;
    std::vector<Point> points;
    readInput(&points_count, &cover, &points);
    long double result = solve(points_count, cover, points);
    writeOutput(result);
}
