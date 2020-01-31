#include "Exercise.h"

const map<string, Instrument> Exercise::name_to_instrument = {{"Unbound", Unbound},
                                                              {"Snare",   SnareInst},
                                                              {"Bass",    BassInst},
                                                              {"HiHat",   HiHatInst}};

Exercise::Exercise(const string &file_name) {
    Json::Reader reader;
    Json::Value exercise;
    ifstream file(file_name);
    reader.parse(file, exercise);
    cout << exercise << endl;

    for (const Json::Value &source : exercise["Sources"]) {
        this->add_layer("../resources/" + source.asString() + ".json");
    }

    this->apply_exercises(exercise["Variations"]);
}

void Exercise::add_layer(string exercise) {
    /*Json::Reader reader;
    Json::Value obj;
    ifstream file(exercise);
    reader.parse(file, obj);

    vector<map<Instrument, unique_ptr<Role>>> layer;
    map<Instrument, unique_ptr<Role>> part;
    unique_ptr<Role> role;
    map<string, Instrument>::const_iterator inst;

    cout << obj << endl;

    for (Json::Value chart : obj["Charts"]) {
        part.clear();
        for (Json::Value part_it : chart["Parts"].getMemberNames()) {
            inst = name_to_instrument.find(part_it.asString());
            if (inst == name_to_instrument.end()) {
                throw exception();
            }
            auto time_signature = make_pair(chart["Time Signature"][0].asInt(), chart["Time Signature"][1].asInt());
            if (!chart["Sticking"][part_it.asString()].empty()) {
                part[inst->second] = move(
                        make_unique<Role>(inst->second, chart["Parts"][part_it.asString()].asString(),
                                          chart["Sticking"][part_it.asString()].asString(), time_signature));
            } else {
                part[inst->second] = move(make_unique<Role>(inst->second, chart["Parts"][part_it.asString()].asString(),
                                                            time_signature));

            }
        }
        layer.push_back(move(part));
    }
    m_layers[obj["Name"].asString()] = move(layer);*/
}


vector<tuple<string, int, Json::Value>> Exercise::json_to_exercises_vector(Json::Value exercise_json) {
    vector<tuple<string, int, Json::Value>> exercise;
    for (Json::Value ex : exercise_json) {
        int which = (ex[1].asString() == "All") ? -1 : ex[1].asInt();
        exercise.emplace_back(make_tuple(ex[0].asString(), which, ex[2]));
    }
    return move(exercise);
}

void Exercise::apply_exercises(Json::Value exercise_json) {
    /*vector<tuple<string, int, Json::Value>> choose = this->json_to_exercises_vector(exercise_json);
    for (auto &choice : choose) {
        auto &layer = m_layers[get<0>(choice)];
        auto initial_index = get<1>(choice);
        for (uint8_t index = (initial_index == -1 ? 0 : initial_index); index < layer.size(); index++) {
            auto &chart = layer[index];
            for (auto &inst : chart) {
                // todo: really supports only one instrument in a layer, may be enough for now...
                auto instrument = get<2>(choice)["Instrument"].asString();
                auto role = get<2>(choice)["Role"];
                if (!role.empty()) {
                    inst.second->apply_variations(role);
                }
                //todo: ensure that iterating the layers while changing isn't breakable
                if (ext::exists(Exercise::name_to_instrument, instrument)) {
                    inst.second->set_instrument(Exercise::name_to_instrument.at(instrument));
                    chart[Exercise::name_to_instrument.at(instrument)] = move(inst.second);
                    // unsafe to delete an object when iterating i guess, also limits to 1 instrument.
                    if (inst.first != Exercise::name_to_instrument.at(instrument)) {
                        chart[inst.first] = nullptr;
                    }
                }
                break; //because changing the contained now iterated, can cause skips or double...
            }
            // delete the nullptr we left behind
            for (auto &inst : chart) {
                if (not inst.second) {
                    chart.erase(inst.first);
                }
            }
            if (initial_index != -1) break;
        }
        //add class to apply variations, or just the functionality to role.
    }*/
}

vector<Notations> Exercise::choose_exercises(vector<pair<string, int>> choice) {
    vector<Notations> notations;
    // todo: make sure that it's a real copy, not shallow and no changes are done to the source exercise
    for (const auto &choose : choice) {
        //for (auto &inst : m_layers[choose.first][choose.second]) {
            // notations.emplace_back(Role(*inst.second));
        //}
    }

    return notations;
}
