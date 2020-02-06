#include "waveform.hpp"
#include <cassert>
#include <iostream>
static constexpr double MIN_FREQUENCY = 20.0;  // Hz
static constexpr double SAMPLE_RATEB = 44100.0;  // Hz
static constexpr int MAX_WAVEFORM_LENGTH = 1 + static_cast<int>(SAMPLE_RATEB / MIN_FREQUENCY);




int WaveGenerator::generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    ClearStereoWaveform(out, framesPerBuffer);
    static int i = 0;
    //std::cout << ++i<< " " << statusFlags << std::endl;
    for (Oscillator* osc : oscillators_) {
        osc->SuperimposeNextSamples(out, framesPerBuffer);
    }

    ScaleStereoWaveformVolume(out, framesPerBuffer);

    return paContinue;
}

void WaveGenerator::AddOscillator(Oscillator* osc) {
    oscillators_.push_back(osc);
}

void WaveGenerator::SetAllOscFrequencies(const float new_frequency) {
    std::cout << "New Frequency: "<<new_frequency<<std::endl;
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

inline void WaveGenerator::ScaleStereoWaveformVolume(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;

    for (size_t i=0; i<num_samples; ++i) {
        out[0][i] *= master_volume_;
        out[1][i] *= master_volume_;
    }
}

void WaveGenerator::SetMasterVolume(const float new_amplitude) {
    master_volume_ = new_amplitude;
}

void WaveGenerator::HandleKnobTurn(const unsigned int knob_id, const float knob_value) {
    switch (knob_id) {
        case 34:
            SetMasterVolume(knob_value);
            break;
        default:
            break;
    }
}
