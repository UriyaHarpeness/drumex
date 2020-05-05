#include "BarContainer.h"

BarContainer::BarContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location,
                           const Fraction &offset) : m_rhythm_container(locations, signature, rests_location, offset,
                                                                        {1, 1}) {
    m_rhythm_container.optimize();
    m_rhythm_container.notationize();
    m_rhythm_container.beam();
}

void BarContainer::prepare_padding(Paddings &padding) const {
    m_rhythm_container.prepare_padding(padding);
}

void BarContainer::display(const GlobalLocations &global_locations) const {
    m_rhythm_container.display(global_locations);
}
