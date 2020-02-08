#pragma once

#include "oscillator.hpp"

class Sinewave : public VCO {
public:
    Sinewave(const float sample_rate_hz, const float frequency, const float amplitude);
    inline float computeAt(const uint32_t idx);
    inline void SuperimposeNextSamples(float** out, uint32_t num_samples);

};