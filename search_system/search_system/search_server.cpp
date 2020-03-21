#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <cstring>
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

void SearchServer::AddQueriesStream(
    istream& query_input, ostream& search_results_output
) {
    vector<size_t> docid_count(__MAX_DOC_NUM__);
    for (string current_query; getline(query_input, current_query); ) {
        const auto words = SplitIntoWords(current_query);
        memset(&docid_count[0], 0, __MAX_DOC_NUM__);
        //map<size_t, size_t> docid_count;
        for (const auto& word : words) {
            for (const size_t docid : index.Lookup(word)) {//to change depending on lookup structure
                docid_count[docid]++;
            }
        }

        vector<pair<size_t, size_t>> search_results(__MAX_DOC_NUM__);
        for (int i = 0; i < __MAX_DOC_NUM__; i++) {
            search_results.push_back({i, docid_count[i]});
        }
        auto mid = search_results.size() > 5 ? begin(search_results) + 5 :
            begin(search_results) + search_results.size();
        partial_sort(
            begin(search_results), 
            mid,
            end(search_results),
            [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                int64_t lhs_docid = lhs.first;
                auto lhs_hit_count = lhs.second;
                int64_t rhs_docid = rhs.first;
                auto rhs_hit_count = rhs.second;
                return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
            }
        );
        int not_zero_index = 0;
        while (not_zero_index != 5) {
            if (search_results[not_zero_index].second == 0)
                break;
            not_zero_index++;
        }
        search_results_output << current_query << ':';
        for (auto [docid, hitcount] : Head(search_results, not_zero_index)) {
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
        index[word].push_back(docid);
    }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    }
    else {
        return {};
    }
}
