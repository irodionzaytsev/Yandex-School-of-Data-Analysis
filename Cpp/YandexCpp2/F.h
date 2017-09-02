#include <iostream>
#include <utility>
#include <vector>
#include <string>

struct student {
    std::string first_name;
    std::string last_name;
    int day;
    int month;
    int year;

    student(std::string& st_first_name,
            std::string& st_last_name,
            int st_day,
            int st_month,
            int st_year)
            : first_name(st_first_name),
              last_name(st_last_name),
              day(st_day),
              month(st_month),
              year(st_year) {}
};

void read_input(std::vector<student>* v) {
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string first_name, last_name;
        int day, month, year;
        std::cin >> first_name >> last_name >> day >> month >> year;
        (*v).push_back(student(first_name, last_name, day, month, year));
    }
}

void write_output(std::vector<int>* v) {
    for (int i = 0; i < (*v).size(); ++i) {
        std::cout << (*v)[i];
        if (i + 1 < (*v).size()) {
            std::cout << std::endl;
        }
    }
}

int main() {
    std::vector<student> v;
    read_input(&v);
    int m;
    std::cin >> m;

    for (int i = 0; i < m; ++i) {
        std::string cmd;
        int number;
        std::cin >> cmd >> number;
        if (number > v.size() || number == 0) {
            std::cout << "bad request\n";
            continue;
        }
        if (cmd == "name") {
            std::cout << v[number - 1].first_name
            << " "
            << v[number - 1].last_name
            << std::endl;
        } else if (cmd == "date") {
            std::cout << v[number - 1].day
            << "."
            << v[number - 1].month
            << "."
            << v[number - 1].year
            << std::endl;
        } else {
            std::cout << "bad request\n";
        }
    }
}
