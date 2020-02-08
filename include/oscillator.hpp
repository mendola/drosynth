#pragma once
#include "portaudiocpp/PortAudioCpp.hxx"

class VCO {
public:
    VCO(const float sample_rate_hz, const float frequency, const float amplitude);
    void SetFrequency(const float new_frequency);
    void Detune(const float detune_factor);
    virtual inline float computeAt(const uint32_t idx) = 0;
    virtual inline void SuperimposeNextSamples(float** out, uint32_t num_samples) = 0;
    void SetAmplitude(const float amplitude);
    void SetFrequencyAndAmplitude(const float new_frequency, const float amplitude);
    void NoteOff();
protected:
    // Inputs
    float v_in_;
    float osc_frequency_;
    float detune_factor_;

    // Internal variables
    float continuous_sample_index_;
    float period_in_fractional_samples_;

    // TODO make const
    float sample_rate_hz_;
};
