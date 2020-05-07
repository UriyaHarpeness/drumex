#include "VoiceContainer.h"

VoiceContainer::VoiceContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location)
        : m_signature(signature) {
    Locations bar;
    Fraction next = signature;

    for (const auto &it : locations) {
        if (it.first >= next) {
            m_bars.emplace_back(move(bar), signature, rests_location, next - signature);
            bar.clear();
            next += signature;
            while (next <= it.first) {
                m_bars.push_back({{}, signature, rests_location, next - signature});
                next += signature;
            }
        }
        bar.insert({it.first - next + signature, it.second});
    }
}

void VoiceContainer::prepare_padding(Paddings &padding) const {
    for_each(m_bars.begin(), m_bars.end(), [&padding](const BarContainer &n) { n.prepare_padding(padding); });
}

void
VoiceContainer::display(const GlobalLocations &global_locations, const vector<int> &bar_splits, const int initial_y,
                        const int y_spacing, int start_bar, int end_bar) const {
    int current_y = initial_y;
    for (; start_bar < end_bar; start_bar++) {
        if (find(bar_splits.begin(), bar_splits.end(), start_bar) != bar_splits.end()) {
            current_y += y_spacing;
        }
        m_bars[start_bar].display(global_locations, current_y);
    }
}

VoiceContainerIterator::VoiceContainerIterator(VoiceContainer &voice) : m_voice(voice), m_bar_index(0),
                                                                        m_rhythms_index({}), m_rhythms({}) {
    RhythmContainer *rhythm = &m_voice.get_mutable_bars()[m_bar_index].get_mutable_rhythm();
    m_rhythms.push_back(rhythm);

    while (!rhythm->get_mutable_rhythms().empty()) {
        rhythm = &rhythm->get_mutable_rhythms()[0];
        m_rhythms_index.push_back(0);
        m_rhythms.push_back(rhythm);
    }
}

VoiceContainerIterator &VoiceContainerIterator::operator++() {
    RhythmContainer *rhythm = nullptr;

    for (int i = m_rhythms_index.size() - 1; i >= 0; i--) {
        if (m_rhythms[i]->get_mutable_rhythms().size() == (m_rhythms_index[i]) + 1) {
            m_rhythms_index.pop_back();
            m_rhythms.pop_back();
        } else {
            m_rhythms_index[i]++;
            m_rhythms[i + 1] = &m_rhythms[i]->get_mutable_rhythms()[m_rhythms_index[i]];
            rhythm = m_rhythms[i + 1];
            break;
        }
    }

    if (!rhythm) {
        m_bar_index++;
        if (m_bar_index < m_voice.get_bars().size()) {
            rhythm = &m_voice.get_mutable_bars()[m_bar_index].get_mutable_rhythm();
            m_rhythms[0] = rhythm;
        }
    }

    if (rhythm) {
        while (!rhythm->get_mutable_rhythms().empty()) {
            rhythm = &rhythm->get_mutable_rhythms()[0];
            m_rhythms_index.push_back(0);
            m_rhythms.push_back(rhythm);
        }
    }

    return *this;
}

VoiceContainerIterator VoiceContainerIterator::operator++(int) {
    VoiceContainerIterator voice_iterator = *this;
    ++(*this);
    return voice_iterator;
}

RhythmContainer *VoiceContainerIterator::operator*() {
    cout << "* bar: " << m_bar_index << ", indexes: ";
    for (auto i : m_rhythms_index) {
        cout << i << " ";
    }
    cout << endl;

    return m_rhythms[m_rhythms.size() - 1];
}

VoiceContainerIterator::operator bool() const {
    return (m_bar_index < m_voice.get_bars().size());
}
