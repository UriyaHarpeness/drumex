#include "Part.h"

Part::Part(Actions actions, TimeSignature time_signature) : m_offset(0), m_actions(actions),
                                                            m_time_signature(time_signature) {

}

void Part::convert_time_4_to_6(uint32_t note_value) {
    vector<Action> converted;
    const uint32_t conversion[4] = {0, 2, 3, 5};
    size_t block = 0;
    converted.resize((m_time_signature.second / 4) * 6, make_tuple(Rest, UnboundStick));
    for (size_t index = 0; index < m_actions.size(); index++) {
        block = index / 4;
        converted[(block * 6) + conversion[index % 4]] = m_actions[index];
    }
    // todo: check 2/4 to 3/6 conversion.

    m_actions = converted;
    m_offset = 0;
    m_time_signature.first = m_time_signature.first * 6 / 4;
    m_time_signature.second = m_time_signature.second * 6 / 4;
}

void Part::convert_time_stretch(uint32_t note_value) {
    vector<Action> converted(note_value, make_tuple(Rest, UnboundStick));
    uint8_t ratio = note_value / m_time_signature.second;

    for (size_t index = 0; index < m_time_signature.second; index++) {
        converted[index * ratio] = m_actions[index];
    }
    // todo: only works for full note sum, like 3/3 or 4/4. also only for divisors like 3 -> 6 or 4 -> 12.
    // need to think if strech full signature, or only note value, maybe support both...

    m_actions = converted;
    m_offset = 0;
    m_time_signature.first = note_value;
    m_time_signature.second = note_value;
}

void Part::convert_time_multiply(uint32_t note_value) {
    vector<Action> converted;
    converted.resize(note_value, make_tuple(Rest, UnboundStick));
    for (size_t index = 0; index < m_actions.size(); index++) {
        converted[(index * note_value) / m_time_signature.second] = m_actions[index];
    }
    // todo: check 2/4 to 3/6 conversion.

    m_actions = converted;
    m_offset = 0;
    m_time_signature.first = (m_time_signature.first * note_value) / m_time_signature.second;
    m_time_signature.second = note_value;
}

// Does not support shortening the part, may be unsafe and lose notes.
// todo: check this works good.
void Part::convert_time(uint32_t note_value) {
    if (note_value % this->m_time_signature.second == 0) {
        this->convert_time_multiply(note_value);
    } else if ((this->m_time_signature.second / 4) % (note_value / 6) == 0) {
        // todo: when will it get here?
        this->convert_time_4_to_6(note_value);
        this->convert_time_multiply(note_value);
    } else {
        throw runtime_error("no compatible conversion");
    }
}

Action Part::get_action() {
    return m_actions[m_offset];
}

const Actions &Part::get_actions() const {
    return m_actions;
}

void Part::set_actions(const Actions &actions) {
    m_actions = actions;
}

const TimeSignature &Part::get_time_signature() const {
    return m_time_signature;
}

void Part::set_time_signature(const TimeSignature &time_signature) {
    m_time_signature = time_signature;
}

void Part::proceed() {
    m_offset = (m_offset + 1) % m_time_signature.first;
}
