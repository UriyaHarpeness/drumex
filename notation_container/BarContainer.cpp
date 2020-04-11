#include "BarContainer.h"

BarContainer::BarContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location,
                           const Fraction &offset) : m_rhythm_container(locations, signature, rests_location, offset,
                                                                        {1, 1}) {
    m_rhythm_container.optimize();
}