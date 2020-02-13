#include "Metronome.h"


Metronome::Metronome(vector<Fraction> locations, int tempo, const Fraction &beat) :
        m_tempo(tempo), m_beat(beat), m_locations(move(locations)), m_start(chrono::system_clock::now()),
        m_current_location(m_locations.begin()) {
    m_locations.erase(m_locations.end() - 2, m_locations.end());
}

void Metronome::poll() {
    // Detect lagging (200ms) and kill process, todo: find better solution and find the source.

    auto now = chrono::system_clock::now();
    auto next_beat = get_next_beat_time();

    assert(next_beat + chrono::milliseconds(200) > now);

    this_thread::sleep_until(next_beat);
}

chrono::system_clock::time_point Metronome::get_next_beat_time() {
    chrono::system_clock::time_point next_beat =
            m_start +
            chrono::milliseconds(
                    static_cast<int>(static_cast<double>(*(m_current_location + 1)) * 1000.0 *
                                     60.0 / static_cast<double>(m_tempo) / static_cast<double>(m_beat)));
    if (++m_current_location == m_locations.end()) {
        m_current_location = m_locations.begin();
        m_start = next_beat;
    };
    return next_beat;
}