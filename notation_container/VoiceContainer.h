#pragma once

#include "BarContainer.h"
#include "../time_signature/TimeSignature.h"

using namespace std;

class VoiceContainer {
public:
    VoiceContainer() : m_bars({}) {};

    VoiceContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location);

    void prepare_padding(Paddings &padding) const;

    void display(const GlobalLocations &global_locations, int start_bar, int end_bar) const;

    [[nodiscard]] const inline vector<BarContainer> &get_bars() const { return m_bars; };

    [[nodiscard]] inline vector<BarContainer> &get_mutable_bars() { return m_bars; };

private:
    vector<BarContainer> m_bars;
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
