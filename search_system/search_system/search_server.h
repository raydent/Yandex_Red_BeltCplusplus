#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define __MAX_DOC_NUM__ 50000
class InvertedIndex {
public:
    void Add(const string& document);
    list<size_t> Lookup(const string& word) const;

    const string& GetDocument(size_t id) const {
        return docs[id];
    }

private:
    map<string, list<size_t>> index;
    vector<string> docs;
};

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    InvertedIndex index;
};
