#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <thread>
#include <numeric>
#include <jsoncpp/json/json.h>

#include "../ext.h"
#include "../notation/Notation.h"

using namespace std;

class Metronome {
public:
    Metronome(Notations notation, uint8_t tempo);

    void start();

private:
    void beat();

    uint32_t get_division();

    chrono::system_clock::time_point get_next_beat_time();

    uint8_t m_tempo;
    uint32_t m_counter;
    std::chrono::system_clock::time_point m_start;
    Notations m_notation;
};
