#include "airline_ticket.h"
#include "test_runner.h"
#include <iomanip>
using namespace std;

#define UPDATE_FIELD(ticket, field, values)  {\
    map<string, string>::const_iterator it = values.find(#field);\
    if (it != values.end()){ \
        istringstream is(it->second);\
        is >> ticket.field;\
    }\
}

bool operator<(const Date& lhs, const Date& rhs) {
    return vector<int>{lhs.year, lhs.month, lhs.day} <
        vector<int>{rhs.year, rhs.month, rhs.day};
}
bool operator==(const Date& lhs, const Date& rhs) {
    return (lhs.year == rhs.year) && (lhs.month == rhs.month) && (lhs.day == rhs.day);
}

ostream& operator<<(ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.year <<
        "-" << setw(2) << setfill('0') << date.month <<
        "-" << setw(2) << setfill('0') << date.day;
    return stream;
}


bool operator<(const Time& lhs, const Time& rhs) {
    long long int lhs_ = static_cast<long long int>(lhs.hours) * 60 + static_cast<long long int>(lhs.minutes), 
        rhs_ = static_cast<long long int>(rhs.hours) * 60 + static_cast<long long int>(rhs.minutes);
    return lhs_ < rhs_;
}

bool operator == (const Time& lhs, const Time& rhs) {
    return (lhs.hours == rhs.hours) && (lhs.minutes == rhs.minutes);
}


ostream& operator << (ostream& os, const Time& rhs) {
    os << rhs.hours << " " << rhs.minutes;
    return os;
}

istream& operator >> (istream& is, Time& rhs) {
    char delim;
    is >> rhs.hours >> delim >> rhs.minutes;
    return is;
}

istream& operator >> (istream& is, Date& rhs) {
    char delim;
    is >> rhs.year >> delim >> rhs.month >> delim >> rhs.day;
    return is;
}

/*istream& operator >> (istream& is, int& i) {
    int temp = 0, res = 0;
    while (is >> )
}*/
void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
      {"departure_date", "2018-2-28"},
      {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
    ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
      {"price", "12550"},
      {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
    ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{ 20, 33 }));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}