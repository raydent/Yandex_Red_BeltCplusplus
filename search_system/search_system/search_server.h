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
#include <unordered_map>
#include <future>
using namespace std;

#define __MAX_DOC_NUM__ 50000

template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(move(initial)) {}

    struct Access {
        T& ref_to_value;
        lock_guard<mutex> guard;
    };

    Access GetAccess() {
        return { value, lock_guard(m) };
    }
private:
    mutex m;
    T value;
};


class InvertedIndex {
public:
    InvertedIndex() = default;
    explicit InvertedIndex(istream & document_input);
    const vector<pair<size_t, size_t>>& Lookup(const string& word) const;

    const string& GetDocument(size_t id) const {
        return docs[id];
    }
    const size_t docs_size() const {
        return docs.size();
    }
    auto GetDataSynchconised() {
        return Synchronized(index);
    }
private:
    map<string, vector<pair<size_t, size_t>>> index;
    vector<string> docs;
};


class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input) : index(InvertedIndex(document_input)) {};
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    Synchronized<InvertedIndex> index;
    vector<future<void>> futures;
};

