#pragma once

#include "BarContainer.h"
#include "../time_signature/TimeSignature.h"

using namespace std;

class VoiceContainer {
public:
    VoiceContainer() {};

    VoiceContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location);

    void prepare_padding(Paddings &padding) const;

    void display(const GlobalLocations &global_locations, const vector<int> &bar_splits, const int initial_y,
                 const int y_spacing, int start_bar, int end_bar) const;

    [[nodiscard]] inline const vector<BarContainer> &get_bars() const { return m_bars; };

    [[nodiscard]] inline vector<BarContainer> &get_mutable_bars() { return m_bars; };

    [[nodiscard]] inline const TimeSignature &get_signature() const { return m_signature; };

private:
    vector<BarContainer> m_bars;

    TimeSignature m_signature;
};


class VoiceContainerIterator {
public:
    VoiceContainerIterator(VoiceContainer &voice);

    VoiceContainerIterator &operator++();

    VoiceContainerIterator operator++(int);

    RhythmContainer *operator*();

    explicit operator bool() const;

private:
    VoiceContainer &m_voice;
    size_t m_bar_index;
    vector<size_t> m_rhythms_index;
    vector<RhythmContainer *> m_rhythms;
};
