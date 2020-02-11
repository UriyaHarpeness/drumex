#include "Metronome.h"


Metronome::Metronome(vector<Fraction> locations, int tempo, const Fraction &beat) :
        m_tempo(tempo), m_beat(beat), m_locations(move(locations)), m_start(chrono::system_clock::now()),
        m_current_location(m_locations.begin()) {}

void Metronome::poll() {
    this_thread::sleep_until(get_next_beat_time());
}

chrono::system_clock::time_point Metronome::get_next_beat_time() {
    chrono::system_clock::time_point next_beat =
            m_start +
            chrono::milliseconds(static_cast<int>(static_cast<double>(*m_current_location) * 1000.0 / 60.0 * m_tempo /
                                                  static_cast<double>(m_beat)));
    m_current_location++;
    return next_beat;
}