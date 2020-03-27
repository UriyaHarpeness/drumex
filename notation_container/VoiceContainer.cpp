#include "VoiceContainer.h"

VoiceContainer::VoiceContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location) {
    Locations bar;
    Fraction next = signature;

    for (const auto &it : locations) {
        if (it.first >= next) {
            m_bars.emplace_back(bar, signature, rests_location);
            bar.clear();
            next += signature;
            while (next <= it.first) {
                m_bars.push_back({{}, signature, rests_location});
                next += signature;
            }
        }
        bar.insert({it.first - next + signature, it.second});
    }
}
