#include "RhythmContainer.h"

set<int> RhythmContainer::primes;

RhythmContainer::RhythmContainer(const Locations &locations, const TimeSignature &signature,
                                 Instrument rests_location) {
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
     */

    int most_occurring_rhythm = get_most_occurring_rhythm(locations);
    Fraction beat = get_beat_from_most_occurring_rhythm(most_occurring_rhythm, signature);

    cout << "most_occurring_rhythm: " << most_occurring_rhythm << ", beat: " << beat << endl;

    // Now split according to the most occurring rhythm.
    Locations rhythm_location;
    for (const auto &location : locations) {
        // Just a sample for now.
        rhythm_location.insert(location);
        if ((!static_cast<bool>(location.first)) ||
            (location.first.get_value().second % 2 == 0) ||
            (is_power_of_2((double) location.first.get_value().second / most_occurring_rhythm))) {
            cout << "fitting " << location.first.get_value().second << endl;

            for (const auto &it : rhythm_location) {
                cout << it.first << endl;
            }
            rhythm_location.clear();
        } else {
            cout << "not " << location.first.get_value().second << endl;
        }
    }

    if (locations.empty()) {
        m_locations[{}].insert({{},
                                {Notation(BaseRest, rests_location, signature, {}, {})}});
        return;
    }

    m_locations = move(location::get_ratios(locations));

    // If no ratio starts at the beginning of the bar and the regular notes do not exist
    if ((locations.find({}) == locations.end()) && (m_locations.find({}) == m_locations.end())) {
        cout << "ok" << endl;
    }
    for (const auto &it : m_locations) {
        for (const auto &y : it.second) {
            cout << y.first << endl;
        }
    }

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

int RhythmContainer::get_most_occurring_rhythm(const Locations &locations) {
    map<int, int> prime_factors_counter;
    for (const auto &it : locations) {
        cout << "denominator: " << it.first.get_value().second << endl;
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
    return *prev(prime_factors_by_count.end())->second.begin();
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
