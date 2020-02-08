#include "oscillator.hpp"

#include <iostream>

VCO::VCO(const float sample_rate_hz, const float frequency, const float amplitude) :
    sample_rate_hz_(sample_rate_hz), 
    osc_frequency_(frequency), 
    v_in_(amplitude)
{
    period_in_fractional_samples_ = sample_rate_hz_ / osc_frequency_;
    detune_factor_ = 1.0;
    continuous_sample_index_ = 0.0;
}

void VCO::SetFrequency(const float new_frequency) {
    osc_frequency_ = new_frequency;
    const float prev_period_in_fractional_samples = period_in_fractional_samples_;
    period_in_fractional_samples_ = sample_rate_hz_ / (osc_frequency_ * detune_factor_ );
    continuous_sample_index_ = continuous_sample_index_ * period_in_fractional_samples_ / prev_period_in_fractional_samples;
}

void VCO::Detune(const float detune_factor) {
    detune_factor_ = detune_factor;
}

void VCO::SetAmplitude(const float amplitude) {
    v_in_ = amplitude;
}

void VCO::SetEnabled(const bool state) {
    is_enabled_ = state;
}