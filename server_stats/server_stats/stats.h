#pragma once

#include "http_request.h"
#include <iostream>
#include <string_view>
#include <map>
using namespace std;

class Stats {
public:
    Stats() {
        Methods["GET"];
        Methods["POST"];
        Methods["PUT"];
        Methods["DELETE"];
        Methods["UNKNOWN"];
        Uris["/"];
        Uris["/order"];
        Uris["/product"];
        Uris["/basket"];
        Uris["/help"];
        Uris["unknown"];
    };
    void AddMethod(string_view method);
    void AddUri(string_view uri);
    const map<string_view, int>& GetMethodStats() const {
        return Methods;
    }
    const map<string_view, int>& GetUriStats() const {
        return Uris;
    }
private:
    map<string_view, int> Methods;
    map<string_view, int> Uris;
    map<string_view, int> Protocols;
};

HttpRequest ParseRequest(string_view line);