#include "NotationDisplay.h"

void NotationDisplay::draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length) {
    assert(number > 0);
    bool up = line <= DisplayConstants::direction_line;
    while (number--) {
        if (up) {
            Notation::m_display->draw_rect(x + 13,
                                           staff_y + ((line - tail_length + 4) * DisplayConstants::line_height) -
                                           DisplayConstants::staff_to_0, DisplayConstants::connector_height, length);
            line += 2;
        } else {
            Notation::m_display->draw_rect(x + 5,
                                           staff_y + ((line + tail_length + 7) * DisplayConstants::line_height) - 2 -
                                           DisplayConstants::staff_to_0, DisplayConstants::connector_height, length);
            line -= 2;
        }
    }
}

void NotationDisplay::draw_connected_notes(const int staff_y, const GlobalLocations &global_locations, Fraction offset,
                                           const Voice &notations) {
    assert(notations.size() >= 2);

    // Assumes all note are in the same direction.
    int max_height = notations[0][0].get_line();
    int min_height = notations[0][0].get_line();

    int beams = 0, max_beams = 1;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            if (abs(max_height - DisplayConstants::direction_line) <
                abs(note.get_line() - DisplayConstants::direction_line)) {
                max_height = note.get_line();
            }
            if (abs(min_height - DisplayConstants::direction_line) >
                abs(note.get_line() - DisplayConstants::direction_line)) {
                min_height = note.get_line();
            }
            beams = (-2 - static_cast<int>(note.get_rounded_length()));
            if (max_beams < beams) {
                max_beams = beams;
            }
        }
    }

    int line_relation = max(abs(max_height - DisplayConstants::direction_line - max_beams) -
                            abs(min_height - DisplayConstants::direction_line), 4) + 3;
    // minimum 1.5 real line difference (1 line from the head end)

    int line, tail_length, distance, last_distance = 0;
    Fraction length_end;
    for (const auto &group : notations) {
        //for (distance = 0, length_end = offset + group[0].get_length(); position->first <= length_end; distance += (
        //        position->second[1] + (position + 1)->second[0]), position++);
        offset += group[0].get_length();

        for (const auto &note : group) {
            line = note.get_line();
            tail_length = line_relation + (line - min_height);
            note.display(global_locations.at(offset).first, staff_y, false, tail_length);
            if (note.get_playing() != BaseRest) {
                // Rests in beams keep the previous note beams number.
                beams = (-2 - note.get_rounded_length().ceil_int());
            }
            if (&note == &(group[0])) {
                if (&group < &(notations[notations.size() - 2])) {
                    draw_connectors(global_locations.at(offset).first, staff_y, group[0].get_line(), distance, beams,
                                    tail_length);
                } else if (&group == &(notations[notations.size() - 2])) {
                    if (group[0].get_rounded_length() > DisplayConstants::minimal_supported_fraction) {
                        last_distance = DisplayConstants::minimal_distance;
                    } else {
                        last_distance = distance / 2;
                    }
                    draw_connectors(global_locations.at(offset).first, staff_y, group[0].get_line(),
                                    distance - last_distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    // beam note size, or half if must smaller...
                    draw_connectors(global_locations.at(offset).first - last_distance, staff_y, group[0].get_line(),
                                    last_distance, beams, tail_length);
                }
            }
        }
    }
}

void NotationDisplay::draw_individual_notes(const int staff_y, const GlobalLocations &global_locations,
                                            const Fraction &offset, const Group &group) {
    for (const auto &note : group) {
        note.display(global_locations.at(offset).first, staff_y, true);
    }
}
