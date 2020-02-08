#pragma once
#include "portaudiocpp/PortAudioCpp.hxx"

class Oscillator {
public:
    Oscillator(const float sample_rate_hz, const float frequency, const float amplitude);
    void SetFrequency(const float new_frequency);
    void Detune(const float detune_factor);
    virtual inline float computeAt(const uint32_t idx) = 0;
    virtual inline void SuperimposeNextSamples(float** out, uint32_t num_samples) = 0;
    void SetAmplitude(const float amplitude);
protected:
    float sample_rate_hz_;
    float osc_frequency_;
    float detune_factor_;
    float amplitude_;
    float continuous_sample_index_;
    float period_in_fractional_samples_;
};
