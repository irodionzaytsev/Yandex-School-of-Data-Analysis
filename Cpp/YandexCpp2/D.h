#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>

struct student {
    std::string first_name;
    std::string last_name;
    int day;
    int month;
    int year;
    int points;
    std::vector<std::string> us;

    student(std::string& st_first_name,
            std::string& st_last_name,
            int st_day,
            int st_month,
            int st_year,
            int st_points,
            std::vector<std::string> st_us)
            : first_name(st_first_name),
              last_name(st_last_name),
              day(st_day),
              month(st_month),
              year(st_year),
              points(st_points),
              us(st_us) {}
};

bool comp_points(const student& st1, const student& st2) {
    if (st1.points > st2.points) {
        return true;
    }
    if (st1.points < st2.points) {
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

bool comp_name(const student& st1, const student& st2) {
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

void read_input(std::vector<student>* v,
                std::map<std::string, int>* universities) {
    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::string u;
        int population;
        std::cin >> u >> population;
        (*universities)[u] = population;
    }
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string first_name, last_name;
        int day, month, year, points;
        std::vector<std::string> us;
        std::cin >> first_name >> last_name >> day >> month >> year >> points;
        int us_num;
        std::cin >> us_num;
        for (int j = 0; j < us_num; ++j) {
            std::string un;
            std::cin >> un;
            us.push_back(un);
        }
        (*v).push_back(student(first_name,
                               last_name,
                               day,
                               month,
                               year,
                               points,
                               us));
    }
}

int main() {
    std::vector<student> v;
    std::map<std::string, int> universities;
    read_input(&v, &universities);

    std::sort(v.begin(), v.end(), comp_points);
    std::map<std::string, std::vector<student>> u_entered;
    for (std::map<std::string, int>::iterator it = universities.begin();
         it != universities.end();
         ++it) {
        u_entered[it->first];
    }
    for (int i = 0; i < v.size(); ++i) {
        for (std::string university : v[i].us) {
            if (universities[university] > 0) {
                --universities[university];
                u_entered[university].push_back(v[i]);
                break;
            }
        }
    }
    for (std::map<std::string,
            std::vector<student>>::iterator it = u_entered.begin();
         it != u_entered.end();
         ++it) {
        std::cout << it->first << "\t";
        std::sort((it->second).begin(), (it->second).end(), comp_name);
        for (int j = 0; j < (it->second).size(); ++j) {
            std::cout << (it->second)[j].first_name
            << " "
            << (it->second)[j].last_name
            << "\t";
        }
        std::cout << "\n";
    }
}
