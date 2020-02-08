#pragma once

#include "oscillator.hpp"

class Squarewave : public VCO {
public:
    Squarewave(const float sample_rate_hz, const float frequency, const float amplitude);
    inline float computeAt(const uint32_t idx);
    inline void SuperimposeNextSamples(float** out, uint32_t num_samples);
};