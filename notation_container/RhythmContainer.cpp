#include "RhythmContainer.h"

set<int> RhythmContainer::primes;

RhythmContainer::RhythmContainer(const Locations &locations, const TimeSignature &scope, Instrument rests_location,
                                 const Fraction &offset, const Fraction &ratio) :
        m_locations(locations), m_offset(offset), m_ratio(ratio), m_length(scope * ratio) {
    /**
     * There is only one main rhythm.
     * The prioritized is the normal meter, that matches the time signature, a division of 2 probably.
     * After that, lower polyrhythm first in ascending order.
     * Each polythythm notation notates only a beat of the upper rhythm.
     */

    /**
     * Actions:
     * 1.   Find most occurring rhythm.
     * 2.   Split by most occurring rhythm's beat.
     * 3.   Repeat the steps 1 and 2 until each part (sub-beat) contains only the same rhythm.
     * 4.   Join close parts with the same rhythm and optimize.
     * 5.   * Maybe join separate parts with the same rhythm with only the basic rhythm between them.
     */

    cout << "-------------------- starting rhythm container --------------------" << endl;

    auto most_occurring = get_most_occurring_rhythm(locations);
    m_most_occurring_rhythm = most_occurring.first;
    bool same_rhythm = most_occurring.second;
    m_beat = get_beat_from_most_occurring_rhythm(m_most_occurring_rhythm, scope, ratio);

    // If no notes start at the beginning of the scope.
    if (m_locations.find({}) == m_locations.end()) {
        m_locations.insert({{},
                            {}});
    }
    m_locations.insert({scope, {}});

    cout << "most_occurring_rhythm: " << m_most_occurring_rhythm << ", beat: " << m_beat << ", same: "
         << (same_rhythm ? "yes" : "no") << ", offset: " << m_offset << ", ratio: " << m_ratio << endl;

    // Now split according to the most occurring rhythm.
    Fraction next_beat = m_beat;
    Locations rhythm_location;

    // Split rhythm further.
    if (!same_rhythm) {
        for (const auto &location : m_locations) {
            // Just a sample for now.

            // todo: need to fill the missing beats, the loop may leave beats without notes and processing.
            if (location.first >= next_beat) {
                m_rhythms_containers.emplace_back(move(rhythm_location), (TimeSignature) {4, 4}, rests_location,
                                                  offset + (next_beat - m_beat), ratio / m_most_occurring_rhythm);
                rhythm_location.clear();
                if (next_beat == scope) {
                    break;
                }
                next_beat += m_beat;
                while (next_beat <= location.first) {
                    m_rhythms_containers.emplace_back(move(rhythm_location), (TimeSignature) {4, 4}, rests_location,
                                                      offset + (next_beat - m_beat), ratio / m_most_occurring_rhythm);
                    rhythm_location.clear();
                    if (next_beat == scope) {
                        break;
                    }
                    next_beat += m_beat;
                }
            }

            // The location is relative to the offset and the beat, need to think how this will be represented.
            rhythm_location.insert({(location.first - (next_beat - m_beat)) / m_beat, location.second});
        }
    }

    // todo: next step is joining the same rhythm containers, and after that - re-evaluate the notes with the new locations.
    if (m_rhythms_containers.empty()) {
        cout << "===================== ending rhythm container =====================" << endl;
        return;
    }

    // RhythmContainer uses either locations - which means it in the lowest level and holds real notations, or it
    // contains a vector of RhythmContainers - which means it's just holding a polyrhythm.

    /*
     * 1---2---3---4---5---6---7---
     * 0------0------0------0------
     * 0--0--0--0--
     * 0---0---0---
     *
     * 3 1----2----3----
     * 5 1--2--3--4--5--
     *
     * 5 0-------0-------0-------0-------0-------
     * 4 0----0----0----0----0----0----0----0----
     */

    cout << "===================== ending rhythm container =====================" << endl;
}

void RhythmContainer::optimize() {
    if (m_rhythms_containers.empty()) {
        location::optimize_location(m_locations);
        return;
    }

    auto rhythm_containers = move(m_rhythms_containers);
    m_rhythms_containers.clear();
    auto prev_container = rhythm_containers.begin();
    m_rhythms_containers.push_back(*prev_container);
    for (auto container = next(rhythm_containers.begin()); container != rhythm_containers.end(); container++) {
        if (prev_container->m_most_occurring_rhythm == container->m_most_occurring_rhythm) {
            cout << "same" << endl;
            m_rhythms_containers[m_rhythms_containers.size() - 1].extend(*container);
        } else {
            prev_container = container;
            m_rhythms_containers.push_back(*container);
        }
    }
    for_each(m_rhythms_containers.begin(), m_rhythms_containers.end(), [](RhythmContainer &n) { n.optimize(); });
}

void RhythmContainer::notationize(Instrument rests_location) {
    // What is the meaning of time signature at this point?
    if (m_rhythms_containers.empty()) {
        // Locations does not contain rests at this point.
        m_notations = location::location_to_notation(m_locations, rests_location, m_ratio);

        m_notations = NotationUtils::convert_notation(m_notations, m_length, m_beat,
                                                      (m_most_occurring_rhythm == 2) ? m_ratio : m_ratio /
                                                                                                 m_most_occurring_rhythm);
        return;
    }
    for_each(m_rhythms_containers.begin(), m_rhythms_containers.end(),
             [rests_location](RhythmContainer &n) { n.notationize(rests_location); });
}

void RhythmContainer::beam() {
    /**
     * Beaming rules:
     * 1. Beaming occurs only between 2 or more played notes, and includes the rests between them.
     * 2. Only notes with simple length of under 1/4 can be beamed (meaning 1/5, 1/7 are not beamed, 1/8, 1/9 are beamed).
     * 3. Notes that are beamed together needs to be in one of the following cases:
     *    1. When not under polyrhythm (natural beat), the beamed notes are all in the same beat.
     *    2. When under a polyrhythm, all notes can beamed together, no matter the total length.
     */
    if (m_rhythms_containers.empty()) {
        Fraction beam_limit, offset;
        if (m_ratio == Fraction(1, 1)) {
            beam_limit = m_beat;
        } else {
            beam_limit = m_length;
        }

        auto notation_it = m_notations.begin(), notation_end = m_notations.begin();
        auto first_non_beamable = m_notations.begin(), prev_non_beamable = m_notations.begin();
        int remaining_plays;
        Voice beamed;
        Fraction current_beam = beam_limit;

        while (current_beam != m_length) {
            while (offset < current_beam) {
                NotationUtils::find_first_non_beamable(offset, current_beam, first_non_beamable);

                remaining_plays = NotationUtils::count_remaining_plays(prev_non_beamable, first_non_beamable);

            cout << "g " << (*prev_non_beamable)[0].get_length() << " " << (*first_non_beamable)[0].get_length()
                     << endl;

                prev_non_beamable = first_non_beamable;
                cout << "f " << remaining_plays << " " << offset << " " << current_beam << endl;

                cout << "see 1 " << (notation_it == m_notations.end()) << " " << (notation_it == first_non_beamable)
                     << " " << offset << endl;

                while ((notation_it != first_non_beamable) && ((*notation_it)[0].get_playing() == BaseRest)) {
                    m_beamed_notations.push_back({*notation_it});
                    offset += (*notation_it)[0].get_length();
                    notation_it++;
                }
                cout << "see 2 " << (notation_it == m_notations.end()) << " " << (notation_it == first_non_beamable)
                     << " " << offset << endl;
                cout << "a " << remaining_plays << endl;
                if (remaining_plays > 0) {
                    while (remaining_plays > 0) {
                        remaining_plays -= ((*notation_it)[0].get_playing() == BasePlay) ? 1 : 0;
                        beamed.push_back(*notation_it);
                        offset += (*notation_it)[0].get_length();
                        notation_it++;
                    }
                    m_beamed_notations.push_back(move(beamed));
                    beamed.clear();
                }
                cout << "see 3 " << (notation_it == m_notations.end()) << " " << (notation_it == first_non_beamable)
                     << " " << offset << endl;
                while (notation_it != first_non_beamable) {
                    m_beamed_notations.push_back({*notation_it});
                    offset += (*notation_it)[0].get_length();
                    notation_it++;
                    cout << "see 4 " << (notation_it == m_notations.end()) << " " << (notation_it == first_non_beamable)
                         << " " << offset << endl;
                }
                if (offset < current_beam) {
                    m_beamed_notations.push_back({*notation_it});
                    offset += (*notation_it)[0].get_length();
                    notation_it++;
                    first_non_beamable++;
                }
            }
            while (current_beam <= offset) {
                current_beam += beam_limit;
            }
        }

        cout << m_notations.size() << " - " << m_beamed_notations.size() << endl;

        return;
    }
    for_each(m_rhythms_containers.begin(), m_rhythms_containers.end(), [](RhythmContainer &n) { n.beam(); });
}

void RhythmContainer::prepare_padding(Paddings &padding, int start_padding, int end_padding) const {
    if (m_rhythms_containers.empty()) {
        Fraction offset = m_offset;

        for (const auto &it : m_notations) {
            padding[offset] = NotationUtils::sum_padding(padding[offset], NotationUtils::merge_padding(it));
            cout << "offset " << offset << endl;
            offset += it[0].get_length();
        }
        // todo: see what is the correct spacing.
        // todo: handle this 20 + 10 - 10 stuff more correctly.
        if (m_most_occurring_rhythm == 2) {
            start_padding -= 10;
            end_padding -= 10;
        }
        padding[m_offset] = NotationUtils::sum_padding(padding[m_offset], {start_padding, 0});
        padding[m_offset + m_length] = NotationUtils::sum_padding(padding[m_offset + m_length], {0, end_padding});

        return;
    }
    for (int index = 0; index < m_rhythms_containers.size(); index++) {
        m_rhythms_containers[index].prepare_padding(padding, ((index == 0) ? start_padding : 0) + 10,
                                                    ((index == m_rhythms_containers.size() - 1) ? end_padding : 0) +
                                                    10);
    }
}

void RhythmContainer::display(const GlobalLocations &global_locations, const int y, int start_padding,
                              int end_padding) const {
    cout << "displaying " << m_offset << endl;
    if (m_rhythms_containers.empty()) {
        // todo: see what is the correct spacing.
        if (m_most_occurring_rhythm == 2) {
            start_padding -= 10;
            end_padding -= 10;
        }
    }

    auto start_location = global_locations.at(m_offset);
    auto end_location = prev(global_locations.find(m_offset + m_length))->second;
    cout << y << " " << start_location.first - start_location.second[0] + start_padding
         << " " << end_location.first + end_location.second[1] - end_padding << endl;

    Notation::m_display->draw_rect(start_location.first - start_location.second[0] + start_padding, y, 3, 3);

    if (m_rhythms_containers.empty()) {
        Fraction offset = m_offset;
        for (const auto &it : m_beamed_notations) {
            if (it.size() == 1) {
                NotationDisplay::draw_individual_notes(y, global_locations, offset, it[0]);
            } else {
                NotationDisplay::draw_connected_notes(y, global_locations, offset, it);
                cout << "no connected yet" << endl;
            }
            for (const auto &i : it) {
                offset += i[0].get_length();
            }
        }
    } else {
        for (int index = 0; index < m_rhythms_containers.size(); index++) {
            m_rhythms_containers[index].display(global_locations, y, ((index == 0) ? start_padding : 0) + 10,
                                                ((index == m_rhythms_containers.size() - 1) ? end_padding : 0) + 10);
        }
    }
}

void RhythmContainer::extend(const RhythmContainer &container) {
    // todo: later make sure that multiple layers polyrhythm extension works as expected.
    m_length += container.m_length;
    m_locations.erase(prev(m_locations.end()));
    for (const auto &location : container.m_locations) {
        m_locations.insert({location.first + ((container.m_offset - m_offset) / m_ratio), location.second});
    }
    location::optimize_location(m_locations);
    auto new_start = m_rhythms_containers.end();
    m_rhythms_containers.insert(m_rhythms_containers.begin(), container.m_rhythms_containers.begin(),
                                container.m_rhythms_containers.end());
}

void RhythmContainer::find_primes() {
    if (!primes.empty()) {
        return;
    }

    // Getting all prime numbers (up to 100).
    vector<int> numbers;
    primes = {};
    for (int i = 1; i <= 100; i++) {
        numbers.push_back(i);
    }
    for (int i = 2; i <= 100; i++) {
        if (numbers[i - 1] != -1) {
            primes.insert(i);
            for (int j = i * 2; j <= 100; j += i) numbers[j - 1] = -1;
        }
    }
}

set<int> RhythmContainer::get_prime_factors(int value) {
    find_primes();
    set<int> primes_factors;
    auto primes_it = primes.begin();

    // what to do with 1? like 0/1 or 1/1?
    while (value > 1) {
        if (value % *primes_it == 0) {
            primes_factors.insert(*primes_it);
            value /= *primes_it;
        } else {
            primes_it++;
        }
    }

    return move(primes_factors);
}

pair<int, bool> RhythmContainer::get_most_occurring_rhythm(const Locations &locations) {
    /*
     * Return the most occurring rhythm and if all the notes are in the same rhythm group.
     *
     * Order of return:
     * 1.   Most occurring division.
     * 2.   Lowest number divisor.
     *
     * The rhythms are counted for every location by finding the prime factors, and for each location - the rhythm is
     * the lowest prime factor that isn't 2, and if there are no prime factors (0/1) or the only factor is 2, 2 is the
     * rhythm.
     */
    map<int, int> prime_factors_counter;
    for (const auto &it : locations) {
        cout << "denominator: " << it.first.get_value().second << ", value: " << it.first << endl;
        auto prime_factors = get_prime_factors(it.first.get_value().second);
        if ((prime_factors.size() > 1) && (prime_factors.find(2) != prime_factors.end())) {
            prime_factors.erase(2);
        }
        for (const auto &prime_factor : prime_factors) {
            prime_factors_counter[prime_factor]++;
        }
    }

    map<int, set<int>> prime_factors_by_count;
    for (const auto &it : prime_factors_counter) {
        prime_factors_by_count[it.second].insert(it.first);
    }
    if (prime_factors_by_count.empty()) {
        prime_factors_by_count[1] = {2};
    } else if ((prime_factors_by_count.size() == 2) &&
               (prime_factors_by_count.begin()->second.size() == 1) &&
               (*prime_factors_by_count.begin()->second.begin() == 2)) {
        // If the rhythms for example can all be divided by 2 and 5, and 5 is the most occurring - act like they all
        // divide only by 5, because 2 can always be noted.
        prime_factors_by_count.erase(prime_factors_by_count.begin());
    }

    // Choose the most occurring rhythm.
    return {*prev(prime_factors_by_count.end())->second.begin(), prime_factors_by_count.size() == 1};
}

Fraction RhythmContainer::get_beat_from_most_occurring_rhythm(int most_occurring_rhythm, const TimeSignature &signature,
                                                              const Fraction &ratio) {
    return (((ratio == Fraction(1, 1)) && (most_occurring_rhythm == 2)) ? signature.get_beat() :
            ratio / ((most_occurring_rhythm == 2) ? 4 : most_occurring_rhythm));
}

bool RhythmContainer::is_power_of_2(double value) {
    return (log2(value) == int(log2(value)));
}
