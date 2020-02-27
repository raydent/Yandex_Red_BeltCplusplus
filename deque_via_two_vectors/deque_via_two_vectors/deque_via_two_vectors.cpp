#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
//#include "test_runner.h"
using namespace std;


template <typename T>
class Deque {
	vector<T> front, back;
public:
	Deque(): front(0), back(0){}
	bool Empty() const {
		return (front.empty() && back.empty() ? true : false);
	}
	size_t Size() const {
		return front.size() + back.size();
	}
	const T& operator [] (size_t index) const {
		/*int index = index_;
		if (index != 0)
			index++;*/
		if (front.empty()) {
			return back[index];
		}
		else if (index + 1 > front.size()) {
			return back[index - front.size()];
		}
		else {
			cout << "here" << index << endl;
			return front[front.size() - index - 1];
		}
	}
	T& operator [] (size_t index) {
		/*int index = index_;
		if (index != 0)
			index++;*/
		if (front.empty()) {
			return back[index];
		}
		else if (index + 1 > front.size()) {
			return back[index - front.size()];
		}
		else {
			//cout << "here" << index << endl;
			return front[front.size() - index - 1];
		}
	}
	T& At(size_t index) {
		if (index >= front.size() + back.size()) {
			throw out_of_range(to_string(index) + ">" + to_string(front.size() + back.size()));
		}
		else {
			if (front.empty()) {
				return back[index];
			}
			else if (index + 1 > front.size()) {
				return back[index - front.size()];
			}
			else {
				//cout << "here" << index << endl;
				return front[front.size() - index - 1];
			}
		}
	}
	const T& At(size_t index) const {
		if (index >= front.size() + back.size()) {
			throw out_of_range(to_string(index) + ">" + to_string(front.size() + back.size()));
		}
		else {
			if (front.empty()) {
				return back[index];
			}
			else if (index + 1 > front.size()) {
				return back[index - front.size()];
			}
			else {
				//cout << "here" << index << endl;
				return front[front.size() - index - 1];
			}
		}
	}
	T& Front() {
		if (front.empty() && !back.empty()) {
			return back[0];
		}
		else if (!front.empty())
			return front.back();
	}
	const T& Front() const {
		if (front.empty() && !back.empty()) {
			return back[0];
		}
		else {
			return front.back();
		}
	}
	T& Back() {
		if (back.empty() && !front.empty()) {
			return front[0];
		}
		else if (!back.empty())
			return back.back();
	}
	const T& Back() const {
		if (back.empty() && !front.empty()) {
			return front[0];
		}
		else {
			return back.back();
		}
	}
	void PushBack(T elem) {
		back.push_back(elem);
	}
	void PushFront(T elem) {
		front.push_back(elem);
	}
	/*void temp_Print() {
		for (auto it = rbegin(front); it != rend(front); it++) {
			cout << *it << " ";
		}
		for (auto it = begin(back); it != end(back); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
	void true_temp_print(){
		cout << "front: ";
		for (auto it = begin(front); it != end(front); it++) {
			cout << *it << " ";
		}
		cout << endl;
		cout << "back: ";
		for (auto it = begin(back); it != end(back); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}*/
};



/*void TestPushFront() {
	Deque<string> d;
	d.PushBack("A");
	ASSERT_EQUAL(d.Back(), "A");
	ASSERT_EQUAL(d.Front(), "A");
	ASSERT_EQUAL(d.Size(), 1);
	d.Back() = "B";
	ASSERT_EQUAL(d.Back(), "B");
	ASSERT_EQUAL(d.Front(), "B");
	ASSERT_EQUAL(d.Size(), 1);
//
	Deque<int> db;
	db.PushBack(1);// 1
	db.PushBack(2);// 1 2
	ASSERT_EQUAL(db.Back(), 2);
	ASSERT_EQUAL(db.Front(), 1);
	ASSERT_EQUAL(db.Size(), 2);
	db.Back() = 5; // 1 5
	ASSERT_EQUAL(db.Back(), 5);
	ASSERT_EQUAL(db.Front(), 1);
	ASSERT_EQUAL(db.Size(), 2);
	db.PushFront(-5);// -5 1 5
	db.true_temp_print();
	ASSERT_EQUAL(db.Front(), -5);
	ASSERT_EQUAL(db.Size(), 3);
	ASSERT_EQUAL(db[1], 1); // front size = 1, back size = 2
	ASSERT_EQUAL(db[0], -5);
	ASSERT_EQUAL(db[2], 5);
}*/

int main() {
	/*TestRunner tr;
	tr.RunTest(TestPushFront, "TestPushFront");*/
	/*Deque<string> d;
	d.PushBack("A");
	cout << d.Front() << " " << d.Back() << " " << d.Size() << endl;
	d.Back() = "B";
	cout << d.Front() << " " << d.Back() << " " << d.Size() << endl;*/

}