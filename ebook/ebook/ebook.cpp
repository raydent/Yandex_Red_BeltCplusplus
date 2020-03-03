#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
using namespace std;

/*class ReadingManager {
public:
    ReadingManager()
        : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        sorted_users_(),
        user_positions_(MAX_USER_COUNT_ + 1, -1) {}

    void Read(int user_id, int page_count) {
        if (user_page_counts_[user_id] == 0) {
            AddUser(user_id);
        }
        user_page_counts_[user_id] = page_count;
        int& position = user_positions_[user_id];
        while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
            SwapUsers(position, position - 1);
        }
    }

    double Cheer(int user_id) const {
        if (user_page_counts_[user_id] == 0) {
            return 0;
        }
        const int user_count = GetUserCount();
        if (user_count == 1) {
            return 1;
        }
        const int page_count = user_page_counts_[user_id];
        int position = user_positions_[user_id];
        while (position < user_count &&
            user_page_counts_[sorted_users_[position]] == page_count) {
            ++position;
        }
        if (position == user_count) {
            return 0;
        }
        // По умолчанию деление целочисленное, поэтому
        // нужно привести числитель к типу double.
        // Простой способ сделать это — умножить его на 1.0.
        return (user_count - position) * 1.0 / (user_count - 1);
    }

private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;

    //vector<int> user_page_counts_;
    //vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
    //vector<int> user_positions_; // позиции в векторе sorted_users_
    set<int> sorted_users_;
    map<int, int> user_positions_;
    int GetUserCount() const {
        return sorted_users_.size();
    }
    void AddUser(int user_id) {
       // sorted_users_.push_back(user_id);
       //user_positions_[user_id] = sorted_users_.size() - 1;
            if (user_positions_.count(user_id) == 0) {
                int pos = user_positions_.size();
                user_positions_[user_id] = pos;
            };
    }
    void SwapUsers(int lhs_position, int rhs_position) {
        const int lhs_id = sorted_users_[lhs_position];
        const int rhs_id = sorted_users_[rhs_position];
        swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
        swap(user_positions_[lhs_id], user_positions_[rhs_id]);
    }
};*/
class ReadingManager {
public:
    void Read(int user_id, int page_count) {
        if (user_pos.count(user_id) == 0) {
            //unique_users.insert(user_id);
            users[page_count].insert(user_id);
            user_pos[user_id] = page_count;
        }
        else {
            users[user_pos[user_id]].erase(user_id);
            users[page_count].insert(user_id);
            user_pos[user_id] = page_count;
        }
    }
    double Cheer(int user_id) const {
        if (user_pos.count(user_id) == 0) {
            return 0;
        }
        int user_count = user_pos.size();
        if (user_count == 1) {
            return 1;
        }
        const int page_count = user_pos.at(user_id);
        auto lb = users.lower_bound(page_count);
        int res = 0;
        if (page_count == lb->first) {
            res--;
        }
        for (auto it = lb; it != users.end(); it++) {
            res += it->second.size();
        }
        return static_cast<double>(user_count - res - 1) / static_cast<double>(user_count - 1);
    }
private:
    map<int, int> user_pos;
   // set<int> unique_users;
    map<int, set<int>> users;
};

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}