#include "NotationDisplayUtils.h"

void NotationDisplayUtils::get_display_scope(const VoiceContainer &up, const VoiceContainer &down,
                                             const Fraction &current_location, DisplayVariables &display_variables) {
    display_variables.current_line = (int) distance(display_variables.bars_split.begin(),
                                                    upper_bound(display_variables.bars_split.begin(),
                                                                display_variables.bars_split.end(),
                                                                (int) static_cast<double>(current_location /
                                                                                          up.get_signature())));
    display_variables.start_line = min(display_variables.current_line, ((int) display_variables.bars_split.size() <
                                                                        display_variables.displayable_lines_number) ? 0
                                                                                                                    :
                                                                       (int) display_variables.bars_split.size() -
                                                                       display_variables.displayable_lines_number + 1);
    display_variables.end_line = min(display_variables.start_line + display_variables.displayable_lines_number - 1,
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
                                            DisplayVariables &display_variables) {
    up.display(display_variables);
    down.display(display_variables);
}

void NotationDisplayUtils::prepare_displayable_notation(VoiceContainer &up, VoiceContainer &down,
                                                        DisplayVariables &display_variables) {
    // Asserts two voices are same offset, when using one voice don't use this.
    assert(up.get_bars().size() == down.get_bars().size());

    Paddings up_padding, down_padding, empty_padding;

    up.prepare_empty_padding(empty_padding);
    down.prepare_empty_padding(empty_padding);

    up_padding = empty_padding;
    down_padding = empty_padding;
    up.fill_padding(up_padding);
    down.fill_padding(down_padding);

    display_variables.merged_padding = NotationUtils::merge_padding(up_padding, down_padding);

    display_variables.global_locations = create_global_locations(display_variables.merged_padding, up.get_signature());

    display_variables.bars_sizes = calculate_bars_sizes(display_variables.merged_padding, up.get_signature());
    display_variables.bars_split = split_bars_to_lines(display_variables.bars_sizes,
                                                       DisplayConstants::initial_window_width -
                                                       DisplayConstants::displaying_init_x -
                                                       DisplayConstants::line_x_offset);
    display_variables.maximum_bar_size = *max_element(display_variables.bars_sizes.begin(),
                                                      display_variables.bars_sizes.end());
    display_variables.line_split_locations = split_global_locations_to_lines(display_variables.global_locations,
                                                                             up.get_signature(),
                                                                             display_variables.bars_split);
    display_variables.displayable_lines_number = calculate_displayable_lines_number(
            DisplayConstants::initial_window_height);
}

void
NotationDisplayUtils::process_events(Metronome &m, bool &quit, bool &moved, DisplayVariables &display_variables,
                                     const TimeSignature &signature) {
    // Interactive SDL communication part.
    bool pause = false;
    SDL_Event event;

    // todo: respond to moves and show on screen on pause.
    while (!quit && (SDL_PollEvent(&event) || pause)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
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
            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                auto current_bar = (int) static_cast<double>(m.get_current_location() / signature);
                auto bars_num = (int) static_cast<double>(prev(display_variables.global_locations.end())->first /
                                                          signature);
                auto new_bar = (current_bar == bars_num - 1) ? 0 : current_bar + 1;
                m.set_current_location(signature * new_bar);
                moved = true;
                Log(INFO).Get() << "Moving to Next Measure: " << new_bar << endl;
            } else if (event.key.keysym.sym == SDLK_LEFT) {
                auto current_bar = (int) static_cast<double>(m.get_current_location() / signature);
                auto bars_num = (int) static_cast<double>(prev(display_variables.global_locations.end())->first /
                                                          signature);
                auto new_bar = (current_bar == 0) ? bars_num - 1 : current_bar - 1;
                m.set_current_location(signature * new_bar);
                moved = true;
                Log(INFO).Get() << "Moving to Previous Measure: " << new_bar << endl;
            }
            if (pause) {
                Metronome::pause(250);
            }
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            auto new_size = Notation::m_display->reset_window_size(display_variables.maximum_bar_size);
            display_variables.bars_split = split_bars_to_lines(display_variables.bars_sizes,
                                                               new_size.first -
                                                               DisplayConstants::displaying_init_x -
                                                               DisplayConstants::line_x_offset);
            display_variables.line_split_locations = split_global_locations_to_lines(display_variables.global_locations,
                                                                                     signature,
                                                                                     display_variables.bars_split);
            display_variables.displayable_lines_number = calculate_displayable_lines_number(new_size.second);
            m.reset();
        }
    }
}

void NotationDisplayUtils::continuous_display_notation(const VoiceContainer &up, const VoiceContainer &down,
                                                       DisplayVariables &display_variables, int tempo) {
    vector<Fraction> locations;
    for (const auto &i: display_variables.global_locations) {
        locations.push_back(i.first);
    }
    bool quit = false;
    bool moved;
    int display_count = 0;

    Metronome m(move(locations), tempo, 5);

    while (!quit) {
        Log(DEBUG).Get() << "display count: " << display_count++ << ", current location: " << m.get_current_location()
                         << endl;

        get_display_scope(up, down, m.get_current_location(), display_variables);

        pair<pair<int, int>, Padding> note_location = {
                {display_variables.line_split_locations.at(m.get_current_location()).first,
                 DisplayConstants::displaying_init_y +
                 ((display_variables.current_line % display_variables.displayable_lines_number) *
                  DisplayConstants::staff_lines_spacing)},
                display_variables.line_split_locations.at(m.get_current_location()).second};

        Notation::m_display->clear_screen();

        Notation::m_display->draw_rect(note_location.first.first - note_location.second[0] + 1,
                                       note_location.first.second - DisplayConstants::line_height * 4,
                                       DisplayConstants::line_height * 16,
                                       note_location.second[0] + note_location.second[1],
                                       255, 0, 0, 128);

        for (int i = display_variables.start_line; i <= display_variables.end_line; i++) {
            Notation::m_display->draw_base(DisplayConstants::displaying_init_y +
                                           ((i % display_variables.displayable_lines_number) *
                                            DisplayConstants::staff_lines_spacing),
                                           up.get_signature().get_value().first, up.get_signature().get_value().second);
        }

        display_notation(up, down, display_variables);

        Notation::m_display->present();

        moved = false;
        process_events(m, quit, moved, display_variables, up.get_signature());

        if (!quit && !moved) {
            m.poll();
        }
    }
}

GlobalLocations NotationDisplayUtils::create_global_locations(const Paddings &padding, const TimeSignature &signature) {
    GlobalLocations global_locations;
    int offset = 0;
    Fraction padding_end = Fraction((int) static_cast<double>(prev(padding.end())->first / signature)) * signature;
    Padding distance, second_distance;

    for (auto it = padding.begin(); it != padding.end(); it++) {
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

    global_locations[padding_end + signature] = {offset, {0, 0}};
    for (const auto &it: global_locations) {
        Log(TRACE).Get() << it.first << ": " << it.second.first << " " << it.second.second[0] << " "
                         << it.second.second[1] << endl;
    }

    return move(global_locations);
}

vector<int> NotationDisplayUtils::calculate_bars_sizes(const Paddings &padding, const TimeSignature &signature) {
    vector<int> bars_sizes((int) static_cast<double>(prev(padding.end())->first / signature) + 1);

    int offset = 0;
    Fraction padding_end = Fraction((int) static_cast<double>(prev(padding.end())->first / signature)) * signature;
    Padding distance;

    for (auto it = padding.begin(); it != padding.end(); it++) {
        if (it == prev(padding.end())) {
            distance = get_distance(padding_end + signature - it->first, it->second);
        } else {
            distance = get_distance(next(it)->first - it->first, it->second);
        }
        if (it->first % signature == 0 && it->first > 0) {
            bars_sizes[(int) static_cast<double>(it->first / signature) - 1] = offset;
            offset = 0;
        }
        offset += distance[0] + distance[1];
    }

    bars_sizes[(int) static_cast<double>(prev(padding.end())->first / signature)] = offset;

    return move(bars_sizes);
}

vector<int> NotationDisplayUtils::split_bars_to_lines(const vector<int> &bars_sizes, int line_size) {
    vector<int> bars_split;

    int offset = 0;
    for (int index = 0; index < bars_sizes.size(); index++) {
        offset += bars_sizes[index];
        if (offset > line_size) {
            bars_split.push_back(index);
            offset = bars_sizes[index];
        }
    }

    return move(bars_split);
}

GlobalLocations NotationDisplayUtils::split_global_locations_to_lines(const GlobalLocations &global_locations,
                                                                      const TimeSignature &signature,
                                                                      const vector<int> &bar_splits) {
    GlobalLocations line_split_locations;
    int locations_offset = 0;
    Padding distance, second_distance;

    for (const auto &it: global_locations) {
        if (it.first % signature == 0 &&
            find(bar_splits.begin(), bar_splits.end(), (int) static_cast<double>(it.first / signature)) !=
            bar_splits.end()) {
            locations_offset = it.second.first;
        }

        line_split_locations[it.first] = {DisplayConstants::displaying_init_x + it.second.first - locations_offset,
                                          it.second.second};
    }

    return move(line_split_locations);
}

int NotationDisplayUtils::calculate_displayable_lines_number(int height) {
    int number = 1;
    height -= DisplayConstants::displaying_init_y + DisplayConstants::staff_lines_spacing / 2;
    // todo: fix Y axis numbers and calibration.

    for (; height >= DisplayConstants::staff_lines_spacing; height -= DisplayConstants::staff_lines_spacing, number++);
    return number;
}

Padding NotationDisplayUtils::get_distance(const Fraction &length, Padding padding) {
    return {padding[0],
            max(static_cast<int>(static_cast<double>(length / DisplayConstants::minimal_supported_fraction) *
                                 DisplayConstants::minimal_distance), DisplayConstants::minimal_distance + padding[1])};
}
