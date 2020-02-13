#pragma once

#include "../ext.h"
#include "../notation/Fraction.h"

#include <chrono>
#include <functional>
#include <iomanip>
#include <thread>
#include <numeric>
#include <utility>
#include <cassert>

using namespace std;

class Metronome {
public:
    Metronome(vector<Fraction> locations, int tempo, const Fraction &beat);

    void poll();

    [[nodiscard]] const vector<Fraction>::iterator get_current_location() const { return m_current_location; }

private:
    chrono::system_clock::time_point get_next_beat_time();

    vector<Fraction> m_locations;
    vector<Fraction>::iterator m_current_location;

    int m_tempo;

    Fraction m_beat;

    // Reset m_start after completing one round.
    std::chrono::system_clock::time_point m_start;
};
