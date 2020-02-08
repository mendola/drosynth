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
    for (VCO* osc : oscillators_) {
        osc->SuperimposeNextSamples(out, framesPerBuffer);
    }

    amplifier_->OperateOnSignal(out, framesPerBuffer);

    ScaleStereoWaveformVolume(out, framesPerBuffer);
    
    return paContinue;
}

void WaveGenerator::AddVCO(VCO* osc) {
    oscillators_.push_back(osc);
}

void WaveGenerator::AddVCA(VCA* vca) {
    amplifier_ = vca;
}

void WaveGenerator::NoteOn(const float new_frequency, const float amplitude) {
    std::cout << "New Frequency: "<<new_frequency<<std::endl;
    for (VCO* osc : oscillators_) {
        osc->SetFrequency(new_frequency);
    }

    amplifier_->SetGainIn(amplitude);
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

void WaveGenerator::SetSingleOscAmplitude(const unsigned int idx, const float amplitude) {
    if (idx < oscillators_.size()) {
        oscillators_.at(idx)->SetAmplitude(amplitude);
    }
}

void WaveGenerator::HandleKnobTurn(const unsigned int knob_id, const float knob_value) {
    switch (knob_id) {
        case 34:
            SetMasterVolume(knob_value);
            break;
        case 35:
            SetSingleOscAmplitude(0, knob_value);
            break;
        case 37:
            SetSingleOscAmplitude(1, knob_value);
            break;
        case 40:
            SetSingleOscAmplitude(2, knob_value);
            break;
        default:
            break;
    }
}

void WaveGenerator::NoteOff() {
    amplifier_->SetGainIn(0.0);
}
