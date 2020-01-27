#pragma once

#include "waveform.hpp"

class Sinewave : private WaveGenerator
{
public:
    Sinewave(const float wavetable_length, const float sample_rate_hz, const float sine_frequency_hz);
    ~Sinewave() {};
    void SetFrequency(const float sine_frequency_hz);
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
protected:
    float sine_frequency_hz_;
    float sample_rate_hz_;
};