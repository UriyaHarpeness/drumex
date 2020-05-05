#pragma once

#include "RhythmContainer.h"
#include "../time_signature/TimeSignature.h"

using namespace std;

class BarContainer {
public:
    BarContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location,
                 const Fraction &offset);

    void prepare_padding(Paddings &padding) const;

    void display(const GlobalLocations &global_locations) const;

    [[nodiscard]] inline RhythmContainer &get_mutable_rhythm() { return m_rhythm_container; };

private:
    RhythmContainer m_rhythm_container;
};
