#include "NotationDisplayUtils.h"

int NotationDisplayUtils::calc_needed_space(const Paddings &distances, const Fraction &offset, const Fraction &length) {
    int distance;
    Fraction length_end;
    auto position = distances.begin();
    while (position->first <= offset) position++;

    //for (distance = 0, length_end = offset + length; position->first <= length_end; distance += (
    //        position->second[1] + (position + 1)->second[0]), position++);

    return distance;
}

void NotationDisplayUtils::get_display_scope(const VoiceContainer &up, const VoiceContainer &down,
                                             const Fraction &current_location, DisplayVariables &display_variables) {
    // todo: check the selected lines and bars are correct.
    // todo: instead of passing tons of parameters, maybe create a struct.
    display_variables.current_line = distance(display_variables.bars_split.begin(),
                                              upper_bound(display_variables.bars_split.begin(),
                                                          display_variables.bars_split.end(),
                                                          (int) static_cast<double>(current_location /
                                                                                    up.get_signature())));
    display_variables.start_line = min(display_variables.current_line, ((int) display_variables.bars_split.size() <
                                                                        DisplayConstants::max_lines_displayed - 1) ? 0 :
                                                                       (int) display_variables.bars_split.size() -
                                                                       DisplayConstants::max_lines_displayed);
    display_variables.end_line = min(display_variables.start_line + DisplayConstants::max_lines_displayed,
                                     (int) display_variables.bars_split.size());
    cout << "current: " << display_variables.current_line << ", start: " << display_variables.start_line << ", end: "
         << display_variables.end_line << endl;

    display_variables.start_bar = (display_variables.start_line == 0) ? 0 : display_variables.bars_split[
            display_variables.start_line - 1];
    display_variables.end_bar = (int) ((display_variables.end_line < display_variables.bars_split.size())
                                       ? display_variables.bars_split[display_variables.end_line]
                                       : up.get_bars().size());
    cout << "start bar: " << display_variables.start_bar << ", end bar: " << display_variables.end_bar << endl;

    for (int i = display_variables.start_line; i <= display_variables.end_line; i++) {
        Notation::m_display->draw_base(20, DisplayConstants::displaying_init_y +
                                           (i * DisplayConstants::staff_lines_spacing),
                                       up.get_signature().get_value().first, up.get_signature().get_value().second);
    }
}

void NotationDisplayUtils::display_notation(const VoiceContainer &up, const VoiceContainer &down,
                                            const Fraction &current_location, DisplayVariables &display_variables) {
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
    up.display(display_variables);
    down.display(display_variables);

    /*int off_x, off_y, edge_padding = 20, lines;
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
    }*/
}

pair<pair<int, int>, Padding>
NotationDisplayUtils::get_note_location(const GroupedNotations &notation, const Paddings &distances,
                                        const Fraction &bar,
                                        const Fraction &location) {
    // todo: optimize this function.
    int off_x = DisplayConstants::displaying_init_x, off_y = DisplayConstants::displaying_init_y, edge_padding = 20;
    Padding padding;
    Fraction offset;

    for (const Voice &note_groups : notation[0]) {
        // Move to the next line to avoid displaying over the staff.
        if (!static_cast<bool>(offset % bar) &&
            (off_x + calc_needed_space(distances, offset, bar) > DisplayConstants::window_width - edge_padding)) {
            off_x = DisplayConstants::displaying_init_x;
            off_y += DisplayConstants::staff_lines_spacing;
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

void NotationDisplayUtils::prepare_displayable_notation(VoiceContainer &up, VoiceContainer &down,
                                                        DisplayVariables &display_variables) {
    // Asserts two voices are same offset, when using one voice don't use this.
    assert(up.get_bars().size() == down.get_bars().size());

    RhythmContainer *rhythm;
    Paddings up_padding, down_padding;

    for (VoiceContainerIterator voice_iterator(up); voice_iterator; voice_iterator++) {
        rhythm = *voice_iterator;
        cout << rhythm->get_offset() << " " << rhythm->get_notations().size() << " "
             << rhythm->get_beamed_notations().size() << endl;
    }

    up.prepare_padding(up_padding);

    for (VoiceContainerIterator voice_iterator(down); voice_iterator; voice_iterator++) {
        rhythm = *voice_iterator;
        cout << rhythm->get_offset() << " " << rhythm->get_notations().size() << " "
             << rhythm->get_beamed_notations().size() << endl;
    }

    down.prepare_padding(down_padding);

    display_variables.merged_padding = NotationUtils::merge_padding(up_padding, down_padding);

    display_variables.global_locations = create_global_locations(display_variables.merged_padding,
                                                                 display_variables.bars_split, up.get_signature());
}

void NotationDisplayUtils::continuous_display_notation(const VoiceContainer &up, const VoiceContainer &down,
                                                       DisplayVariables &display_variables, int tempo) {
    vector<Fraction> locations;
    for (const auto &i : display_variables.global_locations) {
        locations.push_back(i.first);
    }
    SDL_Event event;
    bool quit = false;

    Metronome m(move(locations), tempo, up.get_signature());

    while (!quit) {
        // todo: be responsive to SDL events, to avoid the annoying pop-up.
        cout << *m.get_current_location() << endl;

        get_display_scope(up, down, *m.get_current_location(), display_variables);

        pair<pair<int, int>, Padding> note_location = {
                {display_variables.global_locations.at(*m.get_current_location()).first,
                 DisplayConstants::displaying_init_y +
                 (display_variables.current_line * DisplayConstants::staff_lines_spacing)},
                display_variables.global_locations.at(*m.get_current_location()).second};

        Notation::m_display->clear_screen();

        Notation::m_display->draw_rect(note_location.first.first - note_location.second[0] + 1,
                                       note_location.first.second - DisplayConstants::line_height * 4,
                                       DisplayConstants::line_height * 16,
                                       note_location.second[0] + note_location.second[1],
                                       255, 64, 64, 128);

        for (int i = display_variables.start_line; i <= display_variables.end_line; i++) {
            Notation::m_display->draw_base(20, DisplayConstants::displaying_init_y +
                                               (i * DisplayConstants::staff_lines_spacing),
                                           up.get_signature().get_value().first, up.get_signature().get_value().second);
        }

        display_notation(up, down, *m.get_current_location(), display_variables);

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

GlobalLocations NotationDisplayUtils::create_global_locations(const Paddings &padding, vector<int> &bars_split,
                                                              const TimeSignature &signature) {
    // Also creates bars splitting.
    GlobalLocations global_locations;
    int offset = DisplayConstants::displaying_init_x, bar_offset = 0;

    for (auto it = padding.begin(); it != prev(padding.end()); it++) {
        if (it->first && !static_cast<bool>(it->first % signature)) {
            if (offset > DisplayConstants::displaying_max_x) {
                // Move this bar and the next to another line.
                bars_split.push_back(static_cast<double>(it->first / signature) - 1);
                offset = DisplayConstants::displaying_init_x;
                for (auto i = padding.find(it->first - signature); i != padding.find(it->first); i++) {
                    auto second_distance = get_distance(next(it)->first - it->first, it->second);
                    global_locations[i->first].second = second_distance;
                    offset += second_distance[0];
                    global_locations[i->first].first = offset;
                    offset += second_distance[1];
                }
            }
            bar_offset = offset;
        }

        auto distance = get_distance(next(it)->first - it->first, it->second);
        global_locations[it->first].second = distance;
        offset += distance[0];
        global_locations[it->first].first = offset;
        offset += distance[1];
    }
    global_locations[prev(padding.end())->first].first = offset;
    global_locations[prev(padding.end())->first].second = {0, 0};

    // todo: make sure no duplicate padding.
    for (const auto &it : global_locations) {
        cout << it.first << ": " << it.second.first << " " << it.second.second[0] << " " << it.second.second[1] << endl;
    }

    return move(global_locations);
}

Padding NotationDisplayUtils::get_distance(const Fraction &length, Padding padding) {
    return {padding[0],
            max(static_cast<int>(static_cast<double>(length / DisplayConstants::minimal_supported_fraction) *
                                 DisplayConstants::minimal_distance), DisplayConstants::minimal_distance + padding[1])};
}
