#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <queue>
#include "test_runner.h"
#include "profile.h"
using namespace std;


struct Event {
	int64_t time;
	string hotel_name;
	int64_t client_id;
	int64_t room_count;
};

class BookingManager {
private:
	bool first_in = true;
	int64_t current_time;
	queue<Event> events;
	map<string, uint64_t> rooms;
	map<string, map<int64_t, int64_t>> unique_clients;//first key - hotel name, second key - client id, sec value - last time seen
	//map<string, map<int64_t, map<int64_t, int64_t>>> hotels; //string - hotel name, 1 inmap key - time, 
	void Clear() {
		while (!events.empty() && events.front().time <= current_time - 86400) {
			auto it = events.front();
			unique_clients[it.hotel_name][it.client_id] -= it.room_count;
			if (unique_clients[it.hotel_name][it.client_id] == 0)
				unique_clients[it.hotel_name].erase(it.client_id);
			rooms[it.hotel_name] -= it.room_count;
			events.pop();
		}
	}
public:
	void Book(int64_t time, string hotel_name, int64_t client_id, int64_t room_count) {
		events.push({time, hotel_name, client_id, room_count});
		current_time = time;
		unique_clients[hotel_name][client_id] += room_count;
		rooms[hotel_name] += room_count;
		Clear();
	}
	int64_t Clients(string hotel_name) {
		return unique_clients[hotel_name].size();
	}
	int64_t Rooms(string hotel_name) {
		return rooms[hotel_name];
	}
};


int main() {

	
	int64_t N = 0;
	cin >> N;
	BookingManager bm;
	while (N > 0) {
		N--;
		string op_code;
		cin >> op_code;
		if (op_code == "CLIENTS") {
			string hotel_name;
			cin >> hotel_name;
			cout << bm.Clients(hotel_name) << "\n";
		}
		if (op_code == "ROOMS") {
			string hotel_name;
			cin >> hotel_name;
			cout << bm.Rooms(hotel_name) << "\n";
		}
		if (op_code == "BOOK") {
			int64_t time;
			string hotel_name;
			int64_t client_id;
			int64_t room_count;
			cin >> time >> hotel_name >> client_id >> room_count;
			bm.Book(time, hotel_name, client_id, room_count);
		}
	}
}