#include "VoiceContainer.h"

VoiceContainer::VoiceContainer(const Locations &locations, const TimeSignature &signature, NotesDirection direction)
        : m_signature(signature), m_direction(direction) {
    Locations bar;
    Fraction next = signature;

    for (const auto &it: locations) {
        if (it.first >= next) {
            m_bars.emplace_back(move(bar), signature, direction, next - signature);
            bar.clear();
            next += signature;
            while (next <= it.first) {
                m_bars.push_back({{}, signature, direction, next - signature});
                next += signature;
            }
        }
        bar.insert({it.first - next + signature, it.second});
    }
}

void VoiceContainer::prepare_empty_padding(Paddings &padding) const {
    for_each(m_bars.begin(), m_bars.end(), [&padding](const BarContainer &n) { n.prepare_empty_padding(padding); });
}

void VoiceContainer::fill_padding(Paddings &padding) const {
    for_each(m_bars.begin(), m_bars.end(), [&padding](const BarContainer &n) { n.fill_padding(padding); });
}

void VoiceContainer::display(const DisplayVariables &display_variables) const {
    int line = display_variables.start_line % display_variables.displayable_lines_number;
    for (int bar_index = display_variables.start_bar; bar_index < display_variables.end_bar; bar_index++) {
        if ((bar_index != display_variables.start_bar) &&
            (find(display_variables.bars_split.begin(), display_variables.bars_split.end(), bar_index) !=
             display_variables.bars_split.end())) {
            line = (line + 1) % display_variables.displayable_lines_number;
        }
        m_bars[bar_index].display(display_variables.line_split_locations,
                                  DisplayConstants::displaying_init_y + (DisplayConstants::staff_lines_spacing * line));
        auto last_bar_note = prev(display_variables.line_split_locations.find(Fraction(bar_index + 1) * m_signature));
        Notation::m_display->draw_text((bar_index == m_bars.size() - 1) ? SymDoubleBarLine : SymBarLine,
                                       last_bar_note->second.first + last_bar_note->second.second[1],
                                       DisplayConstants::displaying_init_y + 10 +
                                       (DisplayConstants::staff_lines_spacing * line));
    }
}
