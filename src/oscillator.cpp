#include "oscillator.hpp"

Oscillator::Oscillator(const float sample_rate_hz, const float frequency, const float amplitude) :
    sample_rate_hz_(sample_rate_hz), 
    osc_frequency_(frequency), 
    amplitude_(amplitude)
{
    period_in_samples_ = static_cast<size_t>(sample_rate_hz_/osc_frequency_);
    detune_factor_ = 1.0;
    phase_index_ = 0;
}

void Oscillator::SetFrequency(const float new_frequency) {
    osc_frequency_ = new_frequency;
    period_in_samples_ = static_cast<size_t>(sample_rate_hz_/(osc_frequency_*detune_factor_));
    phase_index_ = 0;//period_in_samples_ % phase_index_;
}


void Oscillator::Detune(const float detune_factor) {
    detune_factor_ = detune_factor;
}

