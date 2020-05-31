#pragma once

#include "Fraction.h"
#include "Notation.h"
#include "NotationUtils.h"
#include "../Enums.h"
#include "../display/Display.h"
#include "../display/DisplayConfigurations.h"
#include "../metronome/Metronome.h"
#include "../notation_container/VoiceContainer.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

namespace NotationDisplayUtils {
    void get_display_scope(const VoiceContainer &up, const VoiceContainer &down,
                           const Fraction &current_location, DisplayVariables &display_variables);

    /**
     * Display the notation.
     */
    void display_notation(const VoiceContainer &up, const VoiceContainer &down, const Fraction &current_location,
                          DisplayVariables &display_variables);

    void prepare_displayable_notation(VoiceContainer &up, VoiceContainer &down, DisplayVariables &display_variables);

    void
    process_events(Metronome &m, bool &quit, const GlobalLocations &global_locations, const TimeSignature &signature);

    void continuous_display_notation(const VoiceContainer &up, const VoiceContainer &down,
                                     DisplayVariables &display_variables, int tempo);

    GlobalLocations
    create_global_locations(const Paddings &padding, vector<int> &bars_split, const TimeSignature &signature);

    Padding get_distance(const Fraction &length, Padding padding);
};
