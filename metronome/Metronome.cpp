#include "Metronome.h"

#include <utility>

Metronome::Metronome(vector<Role> roles, uint8_t tempo, unique_ptr<Mixer> mixer) :
        m_tempo(tempo), m_counter(0), m_start(chrono::system_clock::now()), m_mixer(move(mixer)),
        m_roles(move(roles)) {
    uint32_t division = this->get_division();
    for (auto &role : m_roles) {
        //todo: make sure this changes the part...
        role.get_part().convert_time(division);
    }
}

void Metronome::start() {
    thread([this]() {
        while (true) {
            this->beat();
            this_thread::sleep_until(this->get_next_beat_time());
        }
    }).detach();
}

void Metronome::beat() {
    string played;
    pair<Instrument, Action> new_played;
    for (auto &role: this->m_roles) {
        new_played = role.play();
        ///////////////////
        //todo: playing sounds is problematic, maybe draw a chart
        //if (new_played.second != Stop) {
        //    m_mixer->play_sound(new_played.first);
        //}
        ///////////////////
        played += '0' + get<0>(new_played.second);
        played += '0' + get<1>(new_played.second);
        played += ' ';
        //(get<0>(new_played.second) == Stop) ? '-' : 'o';
    }
    cout << "played: " << played << endl;
}

uint32_t Metronome::get_division() {
    uint32_t division = 1;
    // Ensure that the uint8_t of role does not overlap, maybe casting needed.
    for (auto &role: this->m_roles)
        division = division * role.get_part().get_time_signature().second /
                   gcd(division, role.get_part().get_time_signature().second);
    return division;
}

chrono::system_clock::time_point Metronome::get_next_beat_time() {
    this->m_counter++;
    return this->m_start +
           chrono::milliseconds(this->m_counter * ((60 * 1000 / this->m_tempo) / this->get_division()) * 4);
}