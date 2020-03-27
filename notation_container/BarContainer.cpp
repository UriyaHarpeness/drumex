#include "BarContainer.h"

BarContainer::BarContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location) {
    RhythmContainer(locations, signature, rests_location);
}