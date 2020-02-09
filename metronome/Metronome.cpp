#include "Metronome.h"

#include <utility>

Metronome::Metronome(Notations notation, uint8_t tempo) : m_tempo(tempo), m_counter(0),
                                                          m_start(chrono::system_clock::now()),
                                                          m_notation(move(notation)) {
    uint32_t division = this->get_division();
    for (auto &note_group : notation) {
        //todo: make sure this changes the part...

        // todo: add convert time
        //role.get_part().convert_time(division);
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
    for (auto &notation: m_notation) {
        /*played += '0' + get<0>(new_played.second);
        played += '0' + get<1>(new_played.second);
        played += ' ';*/
        //(get<0>(new_played.second) == Rest) ? '-' : 'o';
    }
    cout << "played: " << played << endl;
}

uint32_t Metronome::get_division() {
    uint32_t division = 1;
    // Ensure that the uint8_t of role does not overlap, maybe casting needed.
    // todo: multiple notation layers
    for (auto &notation: m_notation) {
        /*division = division * role.get_part().get_time_signature().second /
                   gcd(division, role.get_part().get_time_signature().second);*/
    }
    return division;
}

chrono::system_clock::time_point Metronome::get_next_beat_time() {
    this->m_counter++;
    return this->m_start +
           chrono::milliseconds(this->m_counter * ((60 * 1000 / this->m_tempo) / this->get_division()) * 4);
}