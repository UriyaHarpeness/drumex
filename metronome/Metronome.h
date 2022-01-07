#pragma once

#include "../time_signature/TimeSignature.h"

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
    Metronome(vector<Fraction> locations, unsigned int tempo, unsigned char grace_ticks);

    void poll();

    void increase_tempo(char change);

    static void pause(int milliseconds);

    void reset();

    [[nodiscard]] inline unsigned int get_tempo() const { return m_tempo; }

    [[nodiscard]] inline const Fraction &get_current_location() const { return *m_current_location; }

    void set_current_location(const Fraction &location);

private:
    chrono::system_clock::time_point get_next_beat_time();

    vector<Fraction> m_locations;
    vector<Fraction>::iterator m_current_location;

    unsigned int m_tempo;
    unsigned char m_grace_ticks;

    chrono::system_clock::time_point m_start;
};
