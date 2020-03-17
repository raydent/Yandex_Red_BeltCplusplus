
#include <algorithm>
#include <memory>
#include <vector>
#include "test_runner.h"
using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) {
        return;
    }
    else {
        vector<typename RandomIt::value_type> temp_vec{ make_move_iterator(range_begin), make_move_iterator(range_end) };
        auto size_ = (range_end - range_begin) / 3;
        auto iter1 = begin(temp_vec);
        auto iter2 = iter1 + size_;
        auto iter3 = iter2 + size_;
        auto iter4 = temp_vec.end();
        MergeSort(iter1, iter2);
        MergeSort(iter2, iter3);
        MergeSort(iter3, iter4);
        vector<typename RandomIt::value_type> temp_res;
        merge(make_move_iterator(iter1), make_move_iterator(iter2),
            make_move_iterator(iter2), make_move_iterator(iter3),
            back_inserter(temp_res));
        merge(make_move_iterator(begin(temp_res)), make_move_iterator(end(temp_res)),
            make_move_iterator(iter3), make_move_iterator(iter4), range_begin);
    }
}

void TestIntVector() {
    vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
    MergeSort(begin(numbers), end(numbers));
    cout << numbers << endl;
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}