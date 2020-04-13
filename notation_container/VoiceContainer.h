#pragma once

#include "BarContainer.h"
#include "../time_signature/TimeSignature.h"

using namespace std;

class VoiceContainer {
public:
    VoiceContainer() = default;

    VoiceContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location);

private:
    vector<BarContainer> m_bars;
};
