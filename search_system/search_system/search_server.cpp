#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <cstring>
#include <deque>
vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return { istream_iterator<string>(words_input), istream_iterator<string>() };
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document); ) {
        new_index.Add(move(current_document));
    }

    index = move(new_index);
}

class TopFive {
public:
    void Add(size_t index, size_t i) {
        if (size_ < 5) {
            data[index].push_back(i);
            size_++;
            min = data.begin()->first;
        }
        else if (index > min) {
            auto beg = data.begin();
            beg->second.pop_back();
            data[index].push_back(i);
            if (beg->second.empty()) {
                data.erase(data.begin());
            }
            min = data.begin()->first;
        }
    }
    vector<pair<size_t, size_t>> get_data() {
        //return data;
        vector<pair<size_t, size_t>> res;
        /*for (const auto& i : data) {
            for (const auto& i1 : i.second) {
                res.push_back({i1, i.first});
            }
        }*/
        if (!data.empty()) {
            auto it = end(data);
            it--;
            while (it != begin(data)) {
                for (const auto& i : it->second) {
                    res.push_back({ i, it->first });
                }
            }
            for (const auto& i : it->second) {
                res.push_back({ i, it->first });
            }
        }
        return res;
    }
private:
    size_t size_ = 0;
    map<size_t, vector<size_t>> data;
    size_t min = 0;
    //bool overload = true;
};

bool comp(const pair<size_t, deque<size_t>>& lhs,
    const pair<size_t, deque<size_t>>& rhs) {
    return lhs.first < rhs.first;
};
class TestCont {
public:
    TestCont(): data(0), size_(0), min(0){};
    void Add(size_t index, size_t i) {
        deque<size_t> crutch;
        if (size_ < 5) {
            auto it = lower_bound(begin(data), end(data), make_pair(index, crutch), comp);
            //data.insert(it, {});
            if (it == end(data)) {
                data.push_back({ index, {i} });
            }
            else {
                if (it->first == index) {
                    data[it - begin(data)].second.push_back(i);
                }
                else {
                    data.insert(it, { index, {i} });
                }
            }
            min = data[0].first;
            size_++;
        }
        else if (index > min) {
            auto it = lower_bound(begin(data), end(data), make_pair(index, crutch), comp);
            if (it == end(data)) {
                data.push_back({ index, {i} });
            }
            else {
                if (it->first == index) {
                    data[it - begin(data)].second.push_back(i);
                }
                else {
                    data.insert(it, { index, {i} });
                }
            }
            data[0].second.pop_back();
            if (data[0].second.empty())
                data.pop_front();
            min = data[0].first;
        }
    };
    vector<pair<size_t, size_t>> get_data() {
        vector<pair<size_t, size_t>> res;
        if (!data.empty()) {
            auto it = end(data);
            it--;
            while (it != begin(data)) {
                for (const auto& i : it->second) {
                    res.push_back({ i, it->first });
                }
            }
            for (const auto& i : it->second) {
                res.push_back({ i, it->first });
            }
        }
        return res;
    }
private:
    deque<pair<size_t, deque<size_t>>> data;
    size_t size_ = 0;
    size_t min = 0;
};


void SearchServer::AddQueriesStream(
    istream& query_input, ostream& search_results_output
) {
    vector<pair<size_t, size_t>> docid_count(index.docs_size());
    for (string current_query; getline(query_input, current_query); ) {
        const auto words = SplitIntoWords(move(current_query));
        for (size_t i = 0; i < docid_count.size(); i++) {
            docid_count[i] = { i, 0 };
        }
        for (const auto& word : words) {
            for (const auto& i : index.Lookup(word)) {
                docid_count.at(i.first).second += i.second;
            }
        }
        auto mid = docid_count.size() > 5 ? docid_count.begin() + 5 : docid_count.end();
        partial_sort(
            begin(docid_count),
            mid,
            end(docid_count),
            [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                int64_t lhs_docid = lhs.first;
                auto lhs_hit_count = lhs.second;
                int64_t rhs_docid = rhs.first;
                auto rhs_hit_count = rhs.second;
                return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
            }
        );

        search_results_output << current_query << ':';
        for (auto& [docid, hitcount] : Head(docid_count, 5)) {
            if (hitcount == 0) break;
            search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << hitcount << '}';
        }
        search_results_output << endl;
    }
}

void InvertedIndex::Add(const string& document) {
    docs.push_back(document);

    const size_t docid = docs.size() - 1;
    for (const auto& word : SplitIntoWords(document)) {
        auto& temp = index[word];
        if (temp.empty() || temp.back().first != docid) {
            temp.push_back({ docid, 0 });
        }
        temp.back().second++;
    }
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(const string& word) const {
    vector<pair<size_t, size_t>> crutch;
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    }
    else {
        return crutch;
    }
}