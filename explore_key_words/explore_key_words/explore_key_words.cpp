
#include <map>
#include <string>
#include <set>
#include <future>
#include "test_runner.h"
#include "profile.h"

using namespace std;

struct Stats {
    map<string, int> word_frequences;
    /*Stats() = default;
    Stats(string s) {word_frequences[s] = 1; };*/
    auto& operator[](string s){
        return word_frequences[s];
    }
    void operator += (const Stats& other) {
        //word_frequences.merge(other.word_frequences);
        for (const auto& i : other.word_frequences) {
            word_frequences[i.first] += i.second;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    stringstream ss(line);
    string s;
    Stats res;
    while (getline(ss, s, ' ')) {
        //cout << s << endl;
        if (key_words.count(s)) {
            res.word_frequences[s] += 1;
        }
    }
    return res;
}


Stats ExploreLines(const set<string>& key_words, vector<string>::iterator first, vector<string>::iterator last) {
    string s;
    Stats res;
    while (first != last) {
        stringstream ss(move(*first));
        while (getline(ss, s, ' ')) {
            if (key_words.count(s)) {
                res.word_frequences[s] += 1;
            }
        }
        first++;
    }
    return res;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    /*string s;
    Stats res;
    while (getline(input, s)) {
        stringstream ss(move(s));
        while (ss >> s) {
            res.word_frequences[s] += key_words.count(s);
        }
    }
    set<string> keys;
    for (auto& i : res.word_frequences) {
        if (i.second == 0) {
            keys.insert(i.first);
        }
    }
    for (const auto& i : keys) {
        res.word_frequences.erase(i);
    }
    return res;*/
    string s;
    vector<future<Stats>> futs;
    int tread_num = 10;
    vector<string> lines;
    while (getline(input, s)) {
       // futs.push_back(async(ExploreLine, ref(key_words), s));
        lines.push_back(move(s));
    }
    int dist = lines.size();
    int step = dist / tread_num + dist % tread_num;
    auto it = begin(lines);
    auto end_ = end(lines);
    while (it + step < end_) {
        futs.push_back(async(ExploreLines, ref(key_words), it, it + step));
        advance(it, step);
    }
    futs.push_back(async(ExploreLines, ref(key_words), it, end_));
    Stats res;
    for (auto& i : futs) {
        res += i.get();
    }
    return res;
}

void TestBasic() {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
      {"yangle", 6},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
    stringstream ss1;
    ss1 << "    i like     sucks \n";
    const auto stats1 = ExploreKeyWords(key_words, ss1);
    const map<string, int> expected1 = {
        {"sucks", 1}
    };
    AssertEqual(stats1.word_frequences, expected1);

    stringstream ss2;
    ss2 << " yangle, is, for gays\n";
    ss2 << "i hate    this           sucks\n";
    ss2 << "are you fucking kidding me  sucks,           yangle is hate\n";
    const auto stats2 = ExploreKeyWords(key_words, ss2);
    const map<string, int> expected2 = {
        {"sucks", 1}, {"yangle", 1}
    };
    AssertEqual(stats2.word_frequences, expected2);
}



int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}