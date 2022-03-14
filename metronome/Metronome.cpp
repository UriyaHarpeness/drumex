#include "Metronome.h"

Metronome::Metronome(vector<Fraction> locations, unsigned int tempo, unsigned char grace_ticks) :
        m_tempo(tempo), m_grace_ticks(grace_ticks), m_locations(move(locations)), m_start(chrono::system_clock::now()),
        m_current_location(m_locations.begin()) {}

void Metronome::poll() {
    // Detect lagging (100ms) and kill process, todo: find better solution and find the source.
    // todo: make configurable

    auto now = chrono::system_clock::now();
    auto next_beat = get_next_beat_time();

    if (m_grace_ticks > 0) {
        m_grace_ticks--;
    } else if (next_beat + chrono::milliseconds(100) < now) {
        throw runtime_error("Lagging detected (over 100ms)");
        // todo: across the project, exceptions may be better than just runtime errors.
    }

    this_thread::sleep_until(next_beat);
}

void Metronome::increase_tempo(char change) {
    if ((change < 0) && (-change > m_tempo)) {
        m_tempo = 1;
    } else {
        m_tempo += change;
    }
    reset();
}

chrono::system_clock::time_point Metronome::get_next_beat_time() {
    chrono::system_clock::time_point next_beat =
            m_start + chrono::milliseconds(static_cast<int>(static_cast<double>(*(m_current_location + 1)) * 1000.0 *
                                                            60.0 / static_cast<double>(m_tempo) * 4));
    if (*(++m_current_location) >= *prev(m_locations.end())) {
        m_current_location = m_locations.begin();
        m_start = next_beat;
    }
    return next_beat;
}

void Metronome::pause(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void Metronome::reset() {
    auto now = chrono::system_clock::now();

    m_start = now - chrono::milliseconds(
            static_cast<int>(static_cast<double>(*m_current_location) * 1000.0 * 60.0 / static_cast<double>(m_tempo) *
                             4));
}

void Metronome::set_current_location(const Fraction &location) {
    m_current_location = find(m_locations.begin(), m_locations.end(), location);
    reset();
}
