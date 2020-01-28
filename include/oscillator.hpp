#pragma once
#include "portaudiocpp/PortAudioCpp.hxx"

class Oscillator {
public:
    Oscillator(const float sample_rate_hz, const float frequency, const float amplitude);
    void SetFrequency(const float new_frequency);

    virtual inline float computeAt(const uint32_t idx) = 0;

protected:
    float sample_rate_hz_;
    float osc_frequency_;
    float amplitude_;
    size_t period_in_samples_;
};
