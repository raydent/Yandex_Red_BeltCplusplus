
#include "test_runner.h"
#include "student.h"
#include <algorithm>
#include "profile.h"
using namespace std;

//Оптимизируйте эту функцию
bool Compare(const Student& first, const Student& second) {
    //return first.Less(second);
    return first.rating > second.rating;
    //return first.marks > second.marks;
    //return first > second;
    /*double lhs = 0, rhs = 0;
    for (const auto& i : first.marks) {
        lhs += i.second;
    }
    for (const auto& i : second.marks) {
        rhs += i.second;
    }
    return lhs > rhs;*/
}

void TestComparison() {
    LOG_DURATION("start");
    Student newbie{
      "Ivan", "Ivanov", {
        {"c++", 1.0},
        {"algorithms", 3.0}
      },
      2.0
    };

    Student cpp_expert{
      "Petr", "Petrov", {
        {"c++", 9.5},
        {"algorithms", 6.0}
      },
      7.75
    };

    Student guru{
      "Sidor", "Sidorov", {
        {"c++", 10.0},
        {"algorithms", 10.0}
      },
      10.0
    };
    {    
        LOG_DURATION("guru, newbie, cycle");
        for (int i = 0; i < 100000; i++) {
            ASSERT(Compare(guru, newbie));
        }
    }
    {  
        LOG_DURATION("guru, newbie");
        ASSERT(Compare(guru, newbie));
    }
    {
        LOG_DURATION("guru, expert");
        ASSERT(Compare(guru, cpp_expert));
    }
    {
        LOG_DURATION("newbie, expert");
        ASSERT(!Compare(newbie, cpp_expert));
    }
}

void TestSorting() {
    vector<Student> students{
      {"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
      {"Semen", "Semenov", {{"maths", 4.}}, 4.},
      {"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
      {"Petr", "Petrov", {{"maths", 3.}}, 3.},
      {"Alexander", "Alexandrov", {{"maths", 1.}}, 1.}
    };
    sort(students.begin(), students.end(), Compare);
    ASSERT(is_sorted(students.begin(), students.end(),
        [](Student first, Student second) {
            return first.Less(second);
        })
    );
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestComparison);
    RUN_TEST(tr, TestSorting);
    return 0;
}