#include "BarContainer.h"

BarContainer::BarContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location,
                           const Fraction &offset) {
    // todo: need to really pass the offset.
    RhythmContainer(locations, signature, rests_location, offset, {1, 1});
}