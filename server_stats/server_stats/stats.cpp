#include "stats.h"

void Stats::AddMethod(string_view method) {
    if (method == "GET") {
        Methods["GET"]++;
    }
    else if (method == "POST") {
        Methods["POST"]++;
    }
    else if (method == "PUT") {
        Methods["PUT"]++;
    }
    else if (method == "DELETE") {
        Methods["DELETE"]++;
    }
    else
        Methods["UNKNOWN"]++;
}

void Stats::AddUri(string_view uri) {
    //cout << uri << "/" << endl;
    if (uri == "/") {
        Uris["/"]++;
    }
    else if (uri == "/order") {
        Uris["/order"]++;
    }
    else if (uri == "/product") {
        Uris["/product"]++;
    }
    else if (uri == "/basket") {
        Uris["/basket"]++;
    }
    else if (uri == "/help") {
        Uris["/help"]++;
    }
    else
        Uris["unknown"]++;
}

HttpRequest ParseRequest(string_view line) {
    size_t max_size = line.size();
    HttpRequest res;
    const size_t pos_end = line.npos;
    size_t pos = 0;
    while (pos < max_size && !isalpha(line[pos])) {
        pos++;
    }
    if (pos >= max_size) {
        return res;
    }
    size_t slash = pos;
    while (slash < max_size && isalpha(line[slash])) {
        slash++;
    }
    if (slash >= max_size) {
        res.method = line.substr(pos, line.size() - pos);
        cout << "method is" << res.method << "/" << endl;
        return res;
    }
    res.method = line.substr(pos, slash - pos);
    pos = slash + 1;
    slash += 2;
    /*while (slash < max_size && (!('A' <= line[slash] <= 'Z'))) {
        slash++;
    }*/
    slash = line.find(' ', slash);
    if (slash >= max_size) {
        res.uri = line.substr(pos, line.size() - pos);
        cout << "uri " << res.uri << "/" << endl;
        return res;
    }
    res.uri = line.substr(pos, slash - pos);
    slash++;
    res.protocol = line.substr(slash, line.size() - slash);
    return res;
    /*HttpRequest res;
    const size_t pos_end = line.npos;
    size_t pos = 0;
    while (line[pos] == ' ') {
        pos++;
        if (pos == pos_end) {
            res.method = "unknown";
            res.protocol = "unknown";
            res.uri = "unknown";
            return res;
        }
    }
    size_t slash = line.find('/', pos);
    if (slash == pos_end) {
        res.method = line;
        res.protocol = "unknown";
        res.uri = "unknown";
        return res;
    }
    res.method = line.substr(pos, slash - pos - 1);
    //slash++;
    pos = line.find('/', slash + 1);
    if (pos == pos_end) {
        res.protocol = line.substr(pos, line.size() - pos);
        res.uri = "unknown";
        return res;
    }
    res.uri = line.substr(slash, pos - slash - 5);
    pos++;
    if (pos == pos_end) {
        res.protocol = "unknown";
        return res;
    }
    res.protocol = line.substr(pos - 5, line.size() - pos + 5);
    //std::cout << res.protocol << endl;
    return res;*/
}