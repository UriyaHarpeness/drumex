#pragma once

#include "NotationContainer.h"
#include "RhythmContainer.h"
#include "../time_signature/TimeSignature.h"

using namespace std;

class BarContainer : public NotationContainer {
public:
    BarContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location, const Fraction& offset);

private:
    map<Fraction, Locations> m_locations;
};
