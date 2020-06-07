#include "NotationDisplay.h"

array<int, 2> NotationDisplay::get_display_scope(const Voice &beamed) {
    assert(!beamed.empty());
    // Asserts all notes are of the same voice.
    int min_line = beamed[0][0].get_line(), max_line = beamed[0][0].get_line(), max_beams = 0, beams;
    for (const auto &group : beamed) {
        for (const auto &note : group) {
            min_line = min(min_line, note.get_line());
            max_line = max(max_line, note.get_line());

            beams = -2 - static_cast<int>(note.get_simple_length());
            max_beams = max(max_beams, beams);
        }
    }
    // Minimum 1.5 real line difference (1 line from the head end).
    int flag_length = max(abs(abs(max_line - min_line) + (max_beams * 2)) + DisplayConstants::min_stem_length,
                          DisplayConstants::default_stem_length);
    bool up = max_line <= DisplayConstants::direction_line;

    if (up) {
        min_line = max_line - flag_length;
        max_line = max_line;
    } else {
        min_line = min_line;
        max_line = min_line + flag_length;
    }

    return {min_line, max_line};
}

void
NotationDisplay::draw_polyrhythm(int start_x, int end_x, int y, NotesDirection direction, int min_line, int max_line,
                                 int poly_a, int poly_b) {
    string polyrhythm = to_string(poly_a) + ":" + to_string(poly_b);
    // Converts to small numbers.
    for (char &i : polyrhythm) {
        if (('0' <= i) && (i <= '9')) {
            i += 0x50;
        }
    }
    auto size = Notation::m_display->get_size(polyrhythm);

    if (direction == NotesUp) {
        int relative_y =
                (DisplayConstants::staff_to_0 * 2) +
                (DisplayConstants::line_height * (min_line - DisplayConstants::polyrhythm_height_line_spacing));
        Notation::m_display->draw_rect(start_x, y + relative_y, DisplayConstants::polyrhythm_height, 1);
        Notation::m_display->draw_rect(end_x, y + relative_y, DisplayConstants::polyrhythm_height, 1);
        Notation::m_display->draw_rect(start_x, y + relative_y, 1, end_x - start_x);
        Notation::m_display->draw_rect_c((start_x + end_x) / 2, y + relative_y, 10, size.first, 222);
        Notation::m_display->draw_text_c(polyrhythm, (start_x + end_x) / 2, y + relative_y - 5);
    } else {
        int relative_y =
                (DisplayConstants::staff_to_0 * 2) +
                (DisplayConstants::line_height * (max_line + DisplayConstants::polyrhythm_height_line_spacing));
        Notation::m_display->draw_rect(start_x, y + relative_y - DisplayConstants::polyrhythm_height,
                                       DisplayConstants::polyrhythm_height, 1);
        Notation::m_display->draw_rect(end_x, y + relative_y - DisplayConstants::polyrhythm_height,
                                       DisplayConstants::polyrhythm_height, 1);
        Notation::m_display->draw_rect(start_x, y + relative_y - 1, 1, end_x - start_x);
        Notation::m_display->draw_rect_c((start_x + end_x) / 2, y + relative_y - 1, 10, size.first, 222);
        Notation::m_display->draw_text_c(polyrhythm, (start_x + end_x) / 2, y + relative_y - 1 - 5);
    }
}

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

    array<int, 2> display_scope = get_display_scope(notations);
    int beams, stem_length;

    int distance, last_distance = 0;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            stem_length = (note.get_line() <= DisplayConstants::direction_line) ?
                          note.get_line() - display_scope[0] : display_scope[1] - note.get_line();
            note.display(global_locations.at(offset).first, staff_y, false, stem_length);
            if (note.get_playing() != BaseRest) {
                // Rests in beams keep the previous note beams number.
                beams = (-2 - static_cast<int>(note.get_simple_length()));
            }
            distance = global_locations.at(offset + group[0].get_length()).first - global_locations.at(offset).first;

            if (&note == &(group[0])) {
                if (&group < &(notations[notations.size() - 2])) {
                    draw_connectors(global_locations.at(offset).first, staff_y, group[0].get_line(), distance, beams,
                                    stem_length);
                } else if (&group == &(notations[notations.size() - 2])) {
                    // Last beam can be broken if notes have different lengths.
                    if (group[0].get_rounded_length() > DisplayConstants::minimal_supported_fraction) {
                        last_distance = DisplayConstants::minimal_distance;
                    } else {
                        last_distance = distance / 2;
                    }
                    draw_connectors(global_locations.at(offset).first, staff_y, group[0].get_line(),
                                    distance - last_distance, beams, stem_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    draw_connectors(global_locations.at(offset).first - last_distance, staff_y, group[0].get_line(),
                                    last_distance, beams, stem_length);
                }
            }
        }
        offset += group[0].get_length();
    }
}

void NotationDisplay::draw_individual_notes(const int staff_y, const GlobalLocations &global_locations,
                                            const Fraction &offset, const Voice &notations) {
    assert(notations.size() == 1);

    array<int, 2> display_scope = get_display_scope(notations);
    int stem_length;
    for (const auto &note : notations[0]) {
        stem_length = (note.get_line() <= DisplayConstants::direction_line) ?
                      note.get_line() - display_scope[0] : display_scope[1] - note.get_line();

        note.display(global_locations.at(offset).first, staff_y, true, stem_length);
    }
}
