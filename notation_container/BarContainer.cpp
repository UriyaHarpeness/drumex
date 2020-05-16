#include "BarContainer.h"

BarContainer::BarContainer(const Locations &locations, const TimeSignature &signature, NotesDirection direction,
                           const Fraction &offset) : m_rhythm_container(locations, signature, direction, offset,
                                                                        {1, 1}) {
    m_rhythm_container.optimize();
    m_rhythm_container.notationize();
    m_rhythm_container.beam();
}

void BarContainer::prepare_padding(Paddings &padding) const {
    m_rhythm_container.prepare_padding(padding);
}

void BarContainer::display(const GlobalLocations &global_locations, const int y) const {
    m_rhythm_container.display(global_locations, y);
}
