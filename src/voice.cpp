#include "voice.hpp"
#include <iostream>
void Voice::NoteOn(const float new_frequency) {
    voice_freq_ = new_frequency;

    for (VCO* vco : oscillators_) {
        vco->SetFrequency(voice_freq_);
        vco->SetEnabled(true);
    }
    is_triggered_ = true;
}

void Voice::NoteOff() {
    for (VCO* vco : oscillators_) {
        vco->SetEnabled(false);
    }
    is_triggered_ = false;    
}

bool Voice::IsOn() {
    return is_triggered_;
}

void Voice::AddVCO(VCO* vco) {
    oscillators_.push_back(vco);
}

void Voice::SetOneOscillatorAmplitude(const unsigned int idx, const float amplitude) {
    oscillators_.at(idx)->SetAmplitude(amplitude);
}

void Voice::OperateOnSignal(float** out, unsigned int num_samples) {
    for (VCO* osc : oscillators_) {
        osc->SuperimposeNextSamples(out, num_samples);
    }
}