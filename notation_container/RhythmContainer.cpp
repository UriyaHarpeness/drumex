#include "RhythmContainer.h"

set<int> RhythmContainer::primes;

RhythmContainer::RhythmContainer(const Locations &locations, const TimeSignature &scope, Instrument rests_location,
                                 const Fraction &offset, const Fraction &ratio) : m_locations(locations),
                                                                                  m_offset(offset), m_ratio(ratio) {
    /**
     * There is only one main rhythm.
     * The prioritized is the normal meter, that matches the time signature, a division of 2 probably.
     * After that, lower polyrhythm first in ascending order.
     * Each polythythm notation notates only a beat of the upper rhythm.
     */

    /**
     * Actions:
     * 1.   Find most occurring rhythm.
     * 2.   Split by most occurring rhythm's beat
     * 3.   Repeat the steps 1 and 2 until each part (sub-beat) contains only the same rhythm.
     * 4.   Join close parts with the same rhythm and optimize.
     * 5.   * Maybe join separate parts with the same rhythm with only the basic rhythm between them.
     */

    cout << "-------------------- starting rhythm container --------------------" << endl;

    auto most_occurring = get_most_occurring_rhythm(locations);
    m_most_occurring_rhythm = most_occurring.first;
    bool same_rhythm = most_occurring.second;
    Fraction beat = get_beat_from_most_occurring_rhythm(m_most_occurring_rhythm, scope);

    // If no notes start at the beginning of the scope.
    if (m_locations.find({}) == m_locations.end()) {
        m_locations.insert({{},
                            {}});
    }
    m_locations.insert({scope * ratio, {}});

    cout << "most_occurring_rhythm: " << m_most_occurring_rhythm << ", beat: " << beat << ", same: "
         << (same_rhythm ? "yes" : "no") << ", offset: " << m_offset << ", ratio: " << m_ratio << endl;

    // Now split according to the most occurring rhythm.
    Fraction next_beat = beat;
    Locations rhythm_location;

    // Split rhythm further.
    if (!same_rhythm) {
        for (const auto &location : m_locations) {
            // Just a sample for now.

            if (location.first >= next_beat) {
                // todo: need to fill the missing beats, the loop may leave beats without notes and processing.
                m_rhythms_containers.emplace_back(move(rhythm_location), (TimeSignature) {1, 1}, rests_location,
                                                  offset + (next_beat - beat), ratio / m_most_occurring_rhythm);
                rhythm_location.clear();
                if (next_beat == scope) {
                    break;
                }
                next_beat += beat;
                while (next_beat <= location.first) {
                    m_rhythms_containers.emplace_back((Locations) {}, (TimeSignature) {1, 1}, rests_location,
                                                      offset + (next_beat - beat), ratio / m_most_occurring_rhythm);
                    if (next_beat == scope) {
                        break;
                    }
                    next_beat += beat;
                }
            }

            // The location is relative to the offset and the beat, need to think how this will be represented.
            rhythm_location.insert({(location.first - (next_beat - beat)) / beat, move(location.second)});
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
    location::clear_location(m_locations);

    if (m_rhythms_containers.empty()) {
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

void RhythmContainer::extend(const RhythmContainer &container) {
    // todo: later make sure that multiple layers polyrhythm extension works as expected.
    m_locations.erase(prev(m_locations.end()));
    for (const auto &location : container.m_locations) {
        m_locations.insert({location.first + (container.m_offset - m_offset), location.second});
    }
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
            for (int j = i; j <= 100; numbers[(j += i) - 1] = -1);
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
    }

    // Choose the most occurring rhythm.
    return {*prev(prime_factors_by_count.end())->second.begin(), prime_factors_by_count.size() == 1};
}

Fraction
RhythmContainer::get_beat_from_most_occurring_rhythm(int most_occurring_rhythm, const TimeSignature &signature) {
    if (is_power_of_2(most_occurring_rhythm)) {
        return signature.get_beat();
    } else {
        return signature / most_occurring_rhythm;
    }
}

bool RhythmContainer::is_power_of_2(double value) {
    return (log2(value) == int(log2(value)));
}
