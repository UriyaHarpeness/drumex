#include "NotationDisplay.h"

void NotationDisplay::draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length) {
    assert(number > 0);
    bool up = line <= Notation::direction_line;
    while (number--) {
        if (up) {
            Notation::m_display->draw_rect(x + 13, staff_y + ((line - tail_length + 4) * Notation::line_height) -
                                                   Notation::staff_to_0, connector_height, length);
            line += 2;
        } else {
            Notation::m_display->draw_rect(x + 5, staff_y + ((line + tail_length + 7) * Notation::line_height) - 2 -
                                                  Notation::staff_to_0, connector_height, length);
            line -= 2;
        }
    }
}

int NotationDisplay::calc_needed_space(const Paddings &distances, const Fraction &offset, const Fraction &length) {
    int distance;
    Fraction length_end;
    auto position = distances.begin();
    while (position->first <= offset) position++;

    //for (distance = 0, length_end = offset + length; position->first <= length_end; distance += (
    //        position->second[1] + (position + 1)->second[0]), position++);

    return distance;
}

void NotationDisplay::draw_connected_notes(int &x, int staff_y, const Paddings &distances, Fraction offset,
                                           const Voice &notations) {
    assert(notations.size() >= 2);

    // Assumes all note are in the same direction.
    int max_height = notations[0][0].get_line();
    int min_height = notations[0][0].get_line();

    int beams = 0, max_beams = 1;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            if (abs(max_height - Notation::direction_line) < abs(note.get_line() - Notation::direction_line)) {
                max_height = note.get_line();
            }
            if (abs(min_height - Notation::direction_line) > abs(note.get_line() - Notation::direction_line)) {
                min_height = note.get_line();
            }
            beams = (-2 - static_cast<int>(note.get_rounded_length()));
            if (max_beams < beams) {
                max_beams = beams;
            }
        }
    }

    int line_relation =
            max(abs(max_height - Notation::direction_line - max_beams) - abs(min_height - Notation::direction_line),
                4) + 3;
    // minimum 1.5 real line difference (1 line from the head end)
    auto position = distances.begin();
    while (position->first <= offset) position++;

    int line, tail_length, distance, last_distance = 0;
    Fraction length_end;
    x += position->second[0];
    for (const auto &group : notations) {
        //for (distance = 0, length_end = offset + group[0].get_length(); position->first <= length_end; distance += (
        //        position->second[1] + (position + 1)->second[0]), position++);
        offset += group[0].get_length();

        for (const auto &note : group) {
            line = note.get_line();
            tail_length = line_relation + (line - min_height);
            note.display(x, staff_y, false, tail_length);
            if (note.get_playing() != BaseRest) {
                // Rests in beams keep the previous note beams number.
                beams = (-2 - (int) note.get_rounded_length());
            }
            if (&note == &(group[0])) {
                if (&group < &(notations[notations.size() - 2])) {
                    draw_connectors(x, staff_y, group[0].get_line(), distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 2])) {
                    if (group[0].get_rounded_length() > minimal_supported_fraction) {
                        last_distance = Notation::minimal_distance;
                    } else {
                        last_distance = distance / 2;
                    }
                    draw_connectors(x, staff_y, group[0].get_line(), distance - last_distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    // beam note size, or half if must smaller...
                    draw_connectors(x - last_distance, staff_y, group[0].get_line(), last_distance, beams, tail_length);
                }
            }
        }
        x += distance;
    }

    x -= position->second[0];
}

void NotationDisplay::draw_individual_notes(int &x, int staff_y, const Paddings &distances, const Fraction &offset,
                                            const Group &group) {
    // Assumes all note are in the same direction.
    Fraction length_end;
    auto position = distances.begin();
    while (position->first <= offset) position++;

    x += position->second[0];
    for (const auto &note : group) {
        note.display(x, staff_y, true);
    }
    //for (length_end = offset + group[0].get_length();
    //     position->first <= length_end; x += (position->second[1] + (position + 1)->second[0]), position++);
    x -= position->second[0];
}

void NotationDisplay::display_notation(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                                       const int played_line) {
    /*for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            s[0] = (char) (i * 16 + j);
            if (s[0] == 0)continue;

            //d.draw_image("../stuff.png", 400, 0, a * 8, a * 8);
            //d.draw_rect(a, a, 80, 10);

            string k(s);
            d->draw_text(k, 40 + (j * 60), 100 + (i * 60));
            //d.draw_text(to_string(i * 16 + j), 100, 100);
        }
    }*/

    int off_x, off_y, edge_padding = 20, lines;
    Fraction offset;

    // todo: add the notation end double line.
    // todo: skip right to displayed notes, without calculating the distance all over again.
    for (const auto &voice : notation) {
        lines = -1;
        offset = {0, 1};
        off_x = Display::width;
        off_y = displaying_init_y;
        for (const Voice &note_groups : voice) {
            // Move to the next line to avoid displaying over the staff.
            if (!static_cast<bool>(offset % bar) &&
                (off_x + calc_needed_space(distances, offset, bar) > Display::width - edge_padding)) {
                off_x = displaying_init_x;
                off_y += staff_lines_spacing;
                lines++;
                if (lines >= (played_line + max_lines_displayed)) {
                    // Display up to 4 lines.
                    break;
                }
                if ((lines % max_lines_displayed) == 0) {
                    off_y = displaying_init_y;
                }

                if (lines >= played_line) {
                    Notation::m_display->draw_base(edge_padding, off_y, bar.get_value().first, bar.get_value().second);
                }
            }

            if (lines < played_line) {
                off_x += calc_needed_space(distances, offset, NotationUtils::sum_length(note_groups));
            } else {
                if (note_groups.size() > 1) {
                    draw_connected_notes(off_x, off_y, distances, offset, note_groups);
                } else {
                    draw_individual_notes(off_x, off_y, distances, offset, note_groups[0]);
                }
            }

            // The notes do not stretch over bars, so summing the offset will not miss bars.
            offset += NotationUtils::sum_length(note_groups);

            if (!static_cast<bool>(offset % bar)) {
                off_x += 10;
                if (lines >= played_line) {
                    Notation::m_display->draw_text(SymBarLine, off_x, off_y);
                }
                off_x += 10;
            }
            // printing numbers works great.
        }
    }
}

pair<pair<int, int>, Padding>
NotationDisplay::get_note_location(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                                   const Fraction &location) {
    // todo: optimize this function.
    int off_x = displaying_init_x, off_y = displaying_init_y, edge_padding = 20;
    Padding padding;
    Fraction offset;

    for (const Voice &note_groups : notation[0]) {
        // Move to the next line to avoid displaying over the staff.
        if (!static_cast<bool>(offset % bar) &&
            (off_x + calc_needed_space(distances, offset, bar) > Display::width - edge_padding)) {
            off_x = displaying_init_x;
            off_y += staff_lines_spacing;
        }

        int distance;
        Fraction length_end;
        auto position = distances.begin();
        while (position->first <= offset) position++;

        if (offset + NotationUtils::sum_length(note_groups) > location) {
            for (length_end = location;
                 position->first <= length_end; off_x += (position->second[0] + position->second[1]), position++);
            padding = (position)->second;
            break;
        }

        int tmp_x = 0;
        tmp_x += position->second[0];
        //for (distance = 0, length_end = offset + NotationUtils::sum_length(note_groups);
        //     position->first <= length_end; distance += (position->second[1] + (position + 1)->second[0]), position++);
        tmp_x += distance;
        tmp_x -= position->second[0];

        off_x += tmp_x;

        offset += NotationUtils::sum_length(note_groups);

        if (!static_cast<bool>(offset % bar)) {
            off_x += 20;
        }
    }

    return {{off_x, off_y}, padding};
}

void NotationDisplay::continuous_display_notation(const GroupedNotations &notation, const Paddings &distances,
                                                  const TimeSignature &signature, int tempo) {
    vector<Fraction> locations;
    for (const auto &i : distances) {
        locations.push_back(i.first);
    }
    SDL_Event event;
    bool quit = false;

    Metronome m(move(locations), tempo, signature);

    while (!quit) {
        // todo: be responsive to SDL events, to avoid the annoying pop-up.
        cout << *m.get_current_location() << endl;

        auto note_location = get_note_location(notation, distances, signature, *m.get_current_location());

        const int played_line = (note_location.first.second - displaying_init_y) / staff_lines_spacing;

        Notation::m_display->clear_screen();

        Notation::m_display->draw_rect(note_location.first.first + 1,
                                       ((played_line % max_lines_displayed) * staff_lines_spacing) + displaying_init_y -
                                       20, Notation::line_height * 16,
                                       get<0>(note_location.second) + get<1>(note_location.second), 255, 64, 64, 128);

        display_notation(notation, distances, signature, played_line);

        Notation::m_display->present();

        // Interactive SDL communication part.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                cout << "Exiting" << endl;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    cout << "Exiting" << endl;
                    break;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    m.increase_tempo(20);
                    cout << "Increasing Tempo to " << m.get_tempo() << endl;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    m.increase_tempo(-20);
                    cout << "Decreasing Tempo to " << m.get_tempo() << endl;
                }
            }
        }

        m.poll();
    }
}

void NotationDisplay::prepare_displayable_notation(const VoiceContainer &up, const VoiceContainer &down,
                                                   GroupedNotations &notation, Paddings &distances,
                                                   const TimeSignature &bar) {
    // Asserts two voices are same offset, when using one voice don't use this.
    assert(up.get_bars().size() == down.get_bars().size());

    VoiceContainer up_copy = up, down_copy = down;
    RhythmContainer *rhythm;
    Paddings up_padding, down_padding, merged_padding;

    for (VoiceContainerIterator voice_iterator(up_copy); voice_iterator; voice_iterator++) {
        rhythm = *voice_iterator;
        cout << rhythm->get_offset() << " " << rhythm->get_notations().size() << " "
             << rhythm->get_beamed_notations().size() << endl;
    }

    up_copy.prepare_padding(up_padding);

    for (VoiceContainerIterator voice_iterator(down_copy); voice_iterator; voice_iterator++) {
        rhythm = *voice_iterator;
        cout << rhythm->get_offset() << " " << rhythm->get_notations().size() << " "
             << rhythm->get_beamed_notations().size() << endl;
    }

    down_copy.prepare_padding(down_padding);

    merged_padding = NotationUtils::merge_padding(up_padding, down_padding);

    GlobalLocations global_locations = create_global_locations(merged_padding);

    cout << global_locations.size() << endl;
}

GlobalLocations NotationDisplay::create_global_locations(const Paddings &padding) {
    GlobalLocations global_locations;
    int offset = 0;

    for (auto it = padding.begin(); it != prev(padding.end()); it++) {
        auto distance = get_distance(next(it)->first - it->first, it->second);
        offset += distance[0];
        global_locations[it->first] = offset;
        offset += distance[1];
    }
    auto it = prev(padding.end());
    offset += it->second[0];
    global_locations[it->first] = offset;
    offset += it->second[1];

    // todo: make sure no duplicate padding.
    for (const auto &it : global_locations) {
        cout << it.first << ": " << it.second << endl;
    }

    return move(global_locations);
}

Padding NotationDisplay::get_distance(const Fraction &length, Padding padding) {
    return {padding[0],
            max(static_cast<int>(static_cast<double>(length / minimal_supported_fraction) * Notation::minimal_distance),
                Notation::minimal_distance + padding[1])};
}
