#include <cassert>
#include <cmath>
#include "SineWave.hpp"

static constexpr double PI = 3.14159265;

Sinewave::Sinewave(const float wavewaveform_length, const float sample_rate_hz, const float sine_frequency_hz)
{
    sample_rate_hz_ = sample_rate_hz;
    left_phase_idx_ = 0;
    right_phase_idx_ = 0;

    SetFrequency(sine_frequency_hz);
}

void Sinewave::SetFrequency(const float sine_frequency_hz) {
    sine_frequency_hz_ = sine_frequency_hz;
    waveform_size_ = sample_rate_hz_ /  sine_frequency_hz_;
    for (int i = 0; i < waveform_size_; ++i) {
        waveform_[i] = 0.125f * (float)sin(((double)i/(double)waveform_size_)*PI*2.);
    }
}


int Sinewave::generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    for (unsigned int i = 0; i < framesPerBuffer; ++i)
    {
        out[0][i] = waveform_[left_phase_idx_];
        out[1][i] = waveform_[right_phase_idx_];

        left_phase_idx_ += 1;
        if (left_phase_idx_ >= waveform_size_)
            left_phase_idx_ -= waveform_size_;

        right_phase_idx_ += 3;
        if (right_phase_idx_ >= waveform_size_)
            right_phase_idx_ -= waveform_size_;
    }

    return paContinue;
}
