#include <queue>
#include <deque>
#include <set>
#include <stdexcept>
#include "test_runner.h"
using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        if (!freed.empty()) {
            //allocated.push(freed.front());
            T* temp = freed.front();
            allocated.insert(temp);
            freed.pop_front();
            return temp;
        }
        else {
            T* new_obj = new T;
            allocated.insert(new_obj);
            return new_obj;
        }
    }
    T* TryAllocate() {
        if (!freed.empty()) {
            T* temp = freed.front();
            allocated.insert(temp);
            freed.pop_front();
            return temp;
        }
        else {
            return nullptr;
        }
    }

    void Deallocate(T* object) {
        /*bool flag = false;
        for (auto it = allocated.begin(); it != allocated.end(); it++) {
            if (*it == object) {
                flag = true;
                freed.push_back(*it);
                allocated.erase(it);
                break;
            }
        }
        if (!flag)
            throw invalid_argument("no such element");*/
        if (allocated.count(object) == 0) {
            throw invalid_argument("no such element");
        }
        else {
            allocated.erase(object);
            freed.push_back(object);
        }
    }

    ~ObjectPool() {
        for (auto& i : allocated) {
            delete i;
        }
        while (!freed.empty()) {
            delete freed.front();
            freed.pop_front();
        }
    }

private:
    set<T*> allocated;
    deque<T*> freed;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}