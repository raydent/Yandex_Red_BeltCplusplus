#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <mutex>
#include <future>
#include <iostream>
using namespace std;
#define __BUCKET_SIZE__ 100


template <typename T>
int64_t hash_(T key, size_t bucket_count) {
    int sign = key > 0 ? 1 : -1;
    return (sign * key) % bucket_count;
}

template <typename K, typename V>
class ConcurrentMap {
public:
    static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");
    struct Access {
        V& ref_to_value;
        lock_guard<mutex> guard;
    };
    struct MapAccess {
        map<K, V>& ref_to_value;
        lock_guard<mutex> guard;
    };
    explicit ConcurrentMap(size_t bucket_count): count_of_buckets(bucket_count) {
        cout << "constructed" << endl;
        //data.reserve(bucket_count);
    }

    Access operator[](const K& key) {
        //int size_ = data.size();
        //cout << "searching for " << key << endl;
        int64_t nbucket = hash_(key, count_of_buckets);//key / __BUCKET_SIZE__;
        //int nbucket = key / __BUCKET_SIZE__;
        
        auto& curdata = data[nbucket];
        if (curdata.first.empty() || !curdata.first.count(key)) {
            curdata.first[key] = V();
        }
        return { curdata.first[key], lock_guard(curdata.second) };
    }

    MapAccess GetMapAccess(const K& key) {
        return { data[key].first, lock_guard(data[key].second) };
    }


    map<K, V> BuildOrdinaryMap() {
        map<K, V> res;
        cout << "building ordinary map" << endl;
        for (const auto& [k1, p] : data) {
            auto map = GetMapAccess(k1);
            for (const auto& [k, v] : map.ref_to_value) {
                res[k] = v;
            }
        }
        /*for (const auto& [k1, p] : data) {
            for (const auto& map : p.first){
                res[map.first] = operator[](map.first).ref_to_value;
            }
        }*/
        return res;
    }
private:
    //vector<pair<map<K, V>, mutex>> data;
    map<int, pair<map<K, V>, mutex>> data;
    //vector<map<K, V>> data;
    size_t count_of_buckets;
};

void RunConcurrentUpdates(
    ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
    auto kernel = [&cm, key_count](int seed) {
        vector<int> updates(key_count);
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), default_random_engine(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                cm[key].ref_to_value++;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(async(kernel, i));
    }
}

void TestConcurrentUpdate() {
    const size_t thread_count = 3;
    const size_t key_count = 50000;

    ConcurrentMap<int, int> cm(thread_count);
    RunConcurrentUpdates(cm, thread_count, key_count);

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto& [k, v] : result) {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value += 'a';
        }
    };
    auto reader = [&cm] {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : { &r1, &r2 }) {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
            return s.empty() || s == "a" || s == "aa";
            }));
    }
}

void TestSpeedup() {
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    //RUN_TEST(tr, TestReadAndWrite);
    //RUN_TEST(tr, TestSpeedup);
}