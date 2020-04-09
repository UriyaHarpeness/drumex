#include "RhythmContainer.h"

set<int> RhythmContainer::primes;

RhythmContainer::RhythmContainer(const Locations &locations, const TimeSignature &scope, Instrument rests_location,
                                 const Fraction &offset, const Fraction &ratio) : m_locations(locations),
                                                                                  m_offset(offset), m_ratio(ratio) {
    // todo: probably rename from signature to scope

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
    int most_occurring_rhythm = most_occurring.first;
    bool same_rhythm = most_occurring.second;
    Fraction beat = get_beat_from_most_occurring_rhythm(most_occurring_rhythm, scope);

    // If no notes start at the beginning of the scope.
    if (m_locations.find({}) == m_locations.end()) {
        m_locations.insert({{},
                            {}});
    }
    m_locations.insert({scope, {}});

    cout << "most_occurring_rhythm: " << most_occurring_rhythm << ", beat: " << beat << ", same: "
         << (same_rhythm ? "yes" : "no") << ", offset: " << m_offset << ", ratio: " << m_ratio << endl;

    // Now split according to the most occurring rhythm.
    Fraction next = beat;
    Locations rhythm_location;
    vector<RhythmContainer> rhythm_containers;

    // Split rhythm further.
    if (!same_rhythm) {
        for (const auto &location : m_locations) {
            // Just a sample for now.

            if (location.first >= next) {
                // todo: need to fill the missing beats, the loop may leave beats without notes and processing.
                rhythm_containers.emplace_back(move(rhythm_location), (TimeSignature) {1, 1}, rests_location,
                                               offset + (next - beat), ratio / most_occurring_rhythm);
                rhythm_location.clear();
                if (next == scope) {
                    break;
                }
                next += beat;
                while (next <= location.first) {
                    rhythm_containers.emplace_back((Locations) {}, (TimeSignature) {1, 1}, rests_location,
                                                   offset + (next - beat), ratio / most_occurring_rhythm);
                    if (next == scope) {
                        break;
                    }
                    next += beat;
                }
            }

            // The location is relative to the offset and the beat, need to think how this will be represented.
            rhythm_location.insert({(location.first - (next - beat)) / beat, location.second});
        }
    }

    // todo: next step is joining the same rhythm containers, and after that - re-evaluate the notes with the new locations.

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
