#include "NotationDisplayUtils.h"

void NotationDisplayUtils::get_display_scope(const VoiceContainer &up, const VoiceContainer &down,
                                             const Fraction &current_location, DisplayVariables &display_variables) {
    display_variables.current_line = distance(display_variables.bars_split.begin(),
                                              upper_bound(display_variables.bars_split.begin(),
                                                          display_variables.bars_split.end(),
                                                          (int) static_cast<double>(current_location /
                                                                                    up.get_signature())));
    display_variables.start_line = min(display_variables.current_line, ((int) display_variables.bars_split.size() <
                                                                        DisplayConstants::max_lines_displayed) ? 0 :
                                                                       (int) display_variables.bars_split.size() -
                                                                       DisplayConstants::max_lines_displayed + 1);
    display_variables.end_line = min(display_variables.start_line + DisplayConstants::max_lines_displayed - 1,
                                     (int) display_variables.bars_split.size());
    Log(DEBUG).Get() << "current: " << display_variables.current_line << ", start: " << display_variables.start_line
                     << ", end: " << display_variables.end_line << endl;

    display_variables.start_bar = (display_variables.start_line == 0) ? 0 : display_variables.bars_split[
            display_variables.start_line - 1];
    display_variables.end_bar = (int) ((display_variables.end_line < display_variables.bars_split.size())
                                       ? display_variables.bars_split[display_variables.end_line]
                                       : up.get_bars().size());
    Log(DEBUG).Get() << "start bar: " << display_variables.start_bar << ", end bar: " << display_variables.end_bar
                     << endl;
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
    // todo: add the notation end double line.
}

void NotationDisplayUtils::prepare_displayable_notation(VoiceContainer &up, VoiceContainer &down,
                                                        DisplayVariables &display_variables) {
    // Asserts two voices are same offset, when using one voice don't use this.
    assert(up.get_bars().size() == down.get_bars().size());

    RhythmContainer *rhythm;
    Paddings up_padding, down_padding, empty_padding;

    for (VoiceContainerIterator voice_iterator(up); voice_iterator; voice_iterator++) {
        rhythm = *voice_iterator;
        Log(DEBUG).Get() << rhythm->get_offset() << " " << rhythm->get_notations().size() << " "
                         << rhythm->get_beamed_notations().size() << endl;
    }

    for (VoiceContainerIterator voice_iterator(down); voice_iterator; voice_iterator++) {
        rhythm = *voice_iterator;
        Log(DEBUG).Get() << rhythm->get_offset() << " " << rhythm->get_notations().size() << " "
                         << rhythm->get_beamed_notations().size() << endl;
    }

    up.prepare_empty_padding(empty_padding);
    down.prepare_empty_padding(empty_padding);

    up_padding = empty_padding;
    down_padding = empty_padding;
    up.fill_padding(up_padding);
    down.fill_padding(down_padding);

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
    bool quit = false, pause = false;
    int display_count = 0;

    Metronome m(move(locations), tempo, up.get_signature());

    while (!quit) {
        Log(DEBUG).Get() << "display count: " << display_count++ << ", current location: " << *m.get_current_location()
                         << endl;

        get_display_scope(up, down, *m.get_current_location(), display_variables);

        pair<pair<int, int>, Padding> note_location = {
                {display_variables.global_locations.at(*m.get_current_location()).first,
                 DisplayConstants::displaying_init_y +
                 ((display_variables.current_line % DisplayConstants::max_lines_displayed) *
                  DisplayConstants::staff_lines_spacing)},
                display_variables.global_locations.at(*m.get_current_location()).second};

        Notation::m_display->clear_screen();

        Notation::m_display->draw_rect(note_location.first.first - note_location.second[0] + 1,
                                       note_location.first.second - DisplayConstants::line_height * 4,
                                       DisplayConstants::line_height * 16,
                                       note_location.second[0] + note_location.second[1],
                                       255, 64, 64, 128);

        for (int i = display_variables.start_line; i <= display_variables.end_line; i++) {
            Notation::m_display->draw_base(20, DisplayConstants::displaying_init_y +
                                               ((i % DisplayConstants::max_lines_displayed) *
                                                DisplayConstants::staff_lines_spacing),
                                           up.get_signature().get_value().first, up.get_signature().get_value().second);
        }

        display_notation(up, down, *m.get_current_location(), display_variables);

        Notation::m_display->present();

        // Interactive SDL communication part.
        while (!quit && (SDL_PollEvent(&event) || pause)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                Log(INFO).Get() << "Exiting" << endl;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    Log(INFO).Get() << "Exiting" << endl;
                    break;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    pause = !pause;
                    if (pause) {
                        Log(INFO).Get() << "Pause" << endl;
                        m.pause(250);
                    } else {
                        Log(INFO).Get() << "Continue" << endl;
                        m.reset();
                    }
                } else if (event.key.keysym.sym == SDLK_UP) {
                    m.increase_tempo(20);
                    Log(INFO).Get() << "Increasing Tempo to " << m.get_tempo() << endl;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    m.increase_tempo(-20);
                    Log(INFO).Get() << "Decreasing Tempo to " << m.get_tempo() << endl;
                }
            }
        }

        if (!quit) {
            m.poll();
        }
    }
}

GlobalLocations NotationDisplayUtils::create_global_locations(const Paddings &padding, vector<int> &bars_split,
                                                              const TimeSignature &signature) {
    // Also creates bars splitting.
    GlobalLocations global_locations;
    int offset = DisplayConstants::displaying_init_x;
    Fraction padding_end = Fraction((int) static_cast<double>(prev(padding.end())->first / signature)) * signature;
    Padding distance, second_distance;

    for (auto it = padding.begin(); it != padding.end(); it++) {
        if (it->first && !static_cast<bool>(it->first % signature)) {
            if (offset > DisplayConstants::displaying_max_x) {
                // Move this bar and the next to another line.
                bars_split.push_back(static_cast<double>(it->first / signature) - 1);
                offset = DisplayConstants::displaying_init_x;
                for (auto i = padding.find(it->first - signature); i != padding.find(it->first); i++) {
                    second_distance = get_distance(next(i)->first - i->first, i->second);
                    global_locations[i->first].second = second_distance;
                    offset += second_distance[0];
                    global_locations[i->first].first = offset;
                    offset += second_distance[1];
                }
            }
        }

        if (it == prev(padding.end())) {
            distance = get_distance(padding_end + signature - it->first, it->second);
        } else {
            distance = get_distance(next(it)->first - it->first, it->second);
        }
        global_locations[it->first].second = distance;
        offset += distance[0];
        global_locations[it->first].first = offset;
        offset += distance[1];
    }

    if (offset > DisplayConstants::displaying_max_x) {
        // Move this bar and the next to another line.
        bars_split.push_back((int) static_cast<double>(padding_end / signature));
        offset = DisplayConstants::displaying_init_x;
        for (auto i = padding.find(padding_end); i != padding.end(); i++) {
            if (i == prev(padding.end())) {
                second_distance = get_distance(padding_end + signature - i->first, i->second);
            } else {
                second_distance = get_distance(next(i)->first - i->first, i->second);
            }
            global_locations[i->first].second = second_distance;
            offset += second_distance[0];
            global_locations[i->first].first = offset;
            offset += second_distance[1];
        }
    }

    global_locations[padding_end + signature] = {offset, {0, 0}};
    for (const auto &it : global_locations) {
        Log(DEBUG).Get() << it.first << ": " << it.second.first << " " << it.second.second[0] << " "
                         << it.second.second[1] << endl;
    }

    return move(global_locations);
}

Padding NotationDisplayUtils::get_distance(const Fraction &length, Padding padding) {
    return {padding[0],
            max(static_cast<int>(static_cast<double>(length / DisplayConstants::minimal_supported_fraction) *
                                 DisplayConstants::minimal_distance), DisplayConstants::minimal_distance + padding[1])};
}
