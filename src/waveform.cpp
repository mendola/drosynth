#include "waveform.hpp"
#include <cassert>

static constexpr double MIN_FREQUENCY = 20.0;  // Hz
static constexpr double SAMPLE_RATEB = 44100.0;  // Hz
static constexpr int MAX_WAVEFORM_LENGTH = 1 + static_cast<int>(SAMPLE_RATEB / MIN_FREQUENCY);


int WaveGenerator::generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    ClearStereoWaveform(out, framesPerBuffer);

    for (Oscillator* osc : oscillators_) {
        osc->SuperimposeNextSamples(out, framesPerBuffer);
    }

    return paContinue;
}

void WaveGenerator::AddOscillator(Oscillator* osc) {
    oscillators_.push_back(osc);
}

void WaveGenerator::SetAllOscFrequencies(const float new_frequency) {
    for (Oscillator* osc : oscillators_) {
        osc->SetFrequency(new_frequency);
    }
}

inline void WaveGenerator::ClearStereoWaveform(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;

    for (size_t i=0; i<num_samples; ++i) {
        out[0][i] = 0;
        out[1][i] = 0;
    }
}
