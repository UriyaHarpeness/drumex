#pragma once

#include "../Enums.h"

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <tuple>

using namespace std;

class Part {
public:
    Part(Actions actions, TimeSignature time_signature);

    void convert_time_4_to_6(uint32_t note_value);

    void convert_time_multiply(uint32_t note_value);

    void convert_time_stretch(uint32_t note_value);

    void convert_time(uint32_t note_value);

    Action get_action();

    [[nodiscard]]
    const Actions &get_actions() const;

    void set_actions(const Actions &actions);

    [[nodiscard]]
    const TimeSignature &get_time_signature() const;

    void set_time_signature(const TimeSignature &time_signature);

    void proceed();

private:
    Actions m_actions;

    TimeSignature m_time_signature;

    uint32_t m_offset;

    uint32_t m_bar_num;
};