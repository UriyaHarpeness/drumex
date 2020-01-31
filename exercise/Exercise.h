#pragma once

#include "../notation/Notation.h"
#include "../Enums.h"
#include "../ext.h"

#include <map>
#include <string>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std;

class Exercise {
public:
    static const map<string, Instrument> name_to_instrument;

    explicit Exercise(const string &file_name);

    void add_layer(string exercise);

    static vector<tuple<string, int, Json::Value>> json_to_exercises_vector(Json::Value exercise_json);

    void apply_exercises(Json::Value exercise_json);

    vector<Notations> choose_exercises(vector<pair<string, int>> choice);

private:
    map<string, map<int, unique_ptr<Notations>>> m_layers;
};
