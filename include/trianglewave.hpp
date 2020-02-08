
#pragma once

#include "waveform.hpp"
#include "oscillator.hpp"

class TriangleWave : public VCO {
public:
    TriangleWave(const float sample_rate_hz, const float frequency, const float amplitude);
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
    inline float computeAt(const uint32_t idx);
    inline void SuperimposeNextSamples(float** out, uint32_t num_samples);
};