#include <fstream>
#include <iostream>
#include "Reader.h"

Reader::Reader(string file_name) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(file_name);
    reader.parse(f, obj);

    cout << obj << endl;
    cout << obj["Name"] << endl;
    Json::Value &links = obj["parse"]["links"];

    auto *str_links = new string[links.end() - links.begin()];

}