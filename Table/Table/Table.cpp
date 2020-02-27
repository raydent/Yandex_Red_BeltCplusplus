#include "test_runner.h"

using namespace std;

// � еализуйте здесь шаблонный класс Table
template<typename T>
class Table {
    vector<vector<T>> v;
public:
    Table(size_t rows, size_t columns) {
        Resize(rows, columns);
    }
    void Resize(size_t rows, size_t columns) {
        v.resize(rows);
        for (auto& i : v) {
            i.resize(columns);
        }
    }
    const vector<T>& operator [] (size_t index) const {
        return v[index];
    }
    vector<T>& operator [] (size_t index) {
        return v[index];
    }
    pair<size_t, size_t> Size() const {
        return make_pair(v.size(), (v.empty() ? 0 : v[0].size()));
    }
};


void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}