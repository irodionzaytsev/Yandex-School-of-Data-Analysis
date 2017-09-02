#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>

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

bool compdate(const student& st1, const student& st2) {
    if (st1.year < st2.year) {
        return true;
    }
    if (st1.year > st2.year) {
        return false;
    }
    if (st1.month < st2.month) {
        return true;
    }
    if (st1.month > st2.month) {
        return false;
    }
    if (st1.day < st2.day) {
        return true;
    }
    if (st1.day > st2.day) {
        return false;
    }
    if (st1.last_name < st2.last_name) {
        return true;
    }
    if (st1.last_name > st2.last_name) {
        return false;
    }
    if (st1.first_name < st2.first_name) {
        return true;
    }
    if (st1.first_name > st2.first_name) {
        return false;
    }
}

bool compname(const student& st1, const student& st2) {
    if (st1.last_name < st2.last_name) {
        return true;
    }
    if (st1.last_name > st2.last_name) {
        return false;
    }
    if (st1.first_name < st2.first_name) {
        return true;
    }
    if (st1.first_name > st2.first_name) {
        return false;
    }
    if (st1.year < st2.year) {
        return true;
    }
    if (st1.year > st2.year) {
        return false;
    }
    if (st1.month < st2.month) {
        return true;
    }
    if (st1.month > st2.month) {
        return false;
    }
    if (st1.day < st2.day) {
        return true;
    }
    if (st1.day > st2.day) {
        return false;
    }
}

void read_input(std::vector<student>* v, std::string* comp_type) {
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string first_name, last_name;
        int day, month, year;
        std::cin >> first_name >> last_name >> day >> month >> year;
        (*v).push_back(student(first_name, last_name, day, month, year));
    }
    std::cin >> *comp_type;
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
    std::string comp_type;
    read_input(&v, &comp_type);
    if (comp_type == "date") {
        std::sort(v.begin(), v.end(), compdate);
    }
    if (comp_type == "name") {
        std::sort(v.begin(), v.end(), compname);
    }
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i].first_name
                << " "
                << v[i].last_name
                << " "
                << v[i].day
                << "."
                << v[i].month
                << "."
                << v[i].year
                << "\n";
    }
}
