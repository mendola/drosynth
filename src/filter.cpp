#include "filter.hpp"


void Filter::SetCutoffFreq(const float freq) {
    cutoff_frequency_ = freq;
}

void Filter::SetResonance(const float resonance) {
    resonance_ = resonance;
}
