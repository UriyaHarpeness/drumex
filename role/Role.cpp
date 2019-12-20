#include "Role.h"

const map<char, Playing> Role::char_to_playing = {{'o', Regular},
                                                  {'f', Flam},
                                                  {'.', Ghost},
                                                  {'O', Accent}};
const map<char, Sticking> Role::char_to_sticking = {{'r', Right},
                                                    {'l', Left}};

Role::Role(const Role &other) : m_instrument(other.m_instrument), m_part(other.m_part) {

}

Role::Role(Instrument instrument, const string &part, TimeSignature time_signature) :
        m_instrument(instrument), m_part(Role::generate_actions(part), time_signature) {

}

Role::Role(Instrument instrument, const string &part, const string &sticking, TimeSignature time_signature) :
        m_instrument(instrument), m_part(Role::generate_actions(part, sticking), time_signature) {

}

pair<Instrument, Action> Role::play() {
    Action action = m_part.get_action();
    m_part.proceed();
    return make_pair(m_instrument, action);
}

Actions Role::generate_actions(const string &part) {
    Actions actions;
    for (auto &single: part) {
        if (Role::char_to_playing.find(single) != Role::char_to_playing.end()) {
            actions.emplace_back(make_tuple(Role::char_to_playing.at(single), UnboundStick));
        } else {
            actions.emplace_back(make_tuple(Rest, UnboundStick));
        }
    }
    return move(actions);
}

Actions Role::generate_actions(const string &part, const string &sticking) {
    Actions actions;
    map<const char, Playing>::const_iterator play;
    map<const char, Sticking>::const_iterator stick;
    for (size_t i = 0; i < part.length(); i++) {
        play = Role::char_to_playing.find(part[i]);
        stick = Role::char_to_sticking.find(sticking[i]);
        actions.emplace_back(make_tuple((play == Role::char_to_playing.end()) ? Rest : play->second,
                                        (stick == Role::char_to_sticking.end()) ? UnboundStick : stick->second));
    }
    return move(actions);
}

const Instrument &Role::get_instrument() const {
    return m_instrument;
}

void Role::set_instrument(const Instrument &instrument) {
    m_instrument = instrument;
}

Part &Role::get_part() {
    return m_part;
}

void Role::apply_variations(const Json::Value &variations) {
    for (auto variation : variations) {
        cout << "applying variation " << variation[0].asString() << endl;
        variations::name_to_variation.at(variation[0].asString())(m_part, variation);
    }
}
