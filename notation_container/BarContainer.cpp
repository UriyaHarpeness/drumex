#include "BarContainer.h"

BarContainer::BarContainer(const Locations &locations, const TimeSignature &signature, NotesDirection direction,
                           const Fraction &offset) : m_rhythm_container(locations, signature, direction, offset,
                                                                        {1, 1}) {
    m_rhythm_container.optimize();
    m_rhythm_container.notationize();
    m_rhythm_container.beam();
    m_rhythm_container.init_display_scope();
}

void BarContainer::prepare_empty_padding(Paddings &padding) const {
    m_rhythm_container.prepare_empty_padding(padding);
}

void BarContainer::fill_padding(Paddings &padding) const {
    m_rhythm_container.fill_padding(padding);
}

void BarContainer::display(const GlobalLocations &global_locations, const int y) const {
    m_rhythm_container.display(global_locations, y);
}
