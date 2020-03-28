#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <cstring>
#include <deque>
#include <future>
#include <numeric>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return { istream_iterator<string>(words_input), istream_iterator<string>() };
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



InvertedIndex::InvertedIndex(istream& document_input) {
    for (string current_document; getline(document_input, current_document);) {
        docs.push_back(move(current_document));
        size_t docid = docs.size() - 1;
        for (string word : SplitIntoWords(docs.back())) {
            auto& docids = index[word];
            if (!docids.empty() && docids.back().first == docid) {
                docids.back().second++;
            }
            else {
                docids.push_back({ docid, 1 });
            }
        }
    }
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(const string& word) const {
    static const vector<pair<size_t, size_t>> crutch;
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    }
    else {
        return crutch;
    }
}

void UpdateDocumentBaseAsync(istream& document_input, Synchronized<InvertedIndex>& index) {
    InvertedIndex new_index(document_input);
    swap(index.GetAccess().ref_to_value, new_index);
}

void AddQueriesStreamAsync(
    istream& query_input,
    ostream& search_results_output,
    Synchronized<InvertedIndex>& index_handle
) {
    vector<size_t> docid_count;
    vector<int64_t> docids;

    for (string current_query; getline(query_input, current_query);) {
        const auto words = SplitIntoWords(current_query);
        {
            auto access = index_handle.GetAccess();
            const size_t doc_count = access.ref_to_value.docs_size();
            docid_count.assign(doc_count, 0);
            docids.resize(doc_count);

            auto& index = access.ref_to_value;
            for (const auto& word : words) {
                for (const auto& i : index.Lookup(word)) {
                    docid_count[i.first] += i.second;
                }
            }
        }

        iota(docids.begin(), docids.end(), 0);
        partial_sort(begin(docids),
            Head(docids, 5).end(),
            end(docids),
            [&docid_count](int64_t lhs, int64_t rhs) {
                return pair(docid_count[lhs], -lhs) > pair(docid_count[rhs], -rhs);
            }
        );

        search_results_output << current_query << ':';
        for (size_t docid : Head(docids, 5)) {
            const size_t hit_count = docid_count[docid];
            if (hit_count == 0) {
                break;
            }

            search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << hit_count << '}';
        }
        search_results_output << '\n';
    }
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    futures.push_back(async(UpdateDocumentBaseAsync, ref(document_input), ref(index)));
}

void SearchServer::AddQueriesStream(
    istream& query_input, ostream& search_results_output
) {
    futures.push_back(async(AddQueriesStreamAsync, ref(query_input),
            ref(search_results_output), ref(index)));
}