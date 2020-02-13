#include "voice.hpp"

#include <cstring>
#include <iostream>

float Voice::scratch_memory_[2][FRAMES_PER_BUFFER];
float* Voice::scratch_arrays_[2];
float** Voice::scratch_buf_ = NULL;

Voice::Voice() {
    Voice::scratch_arrays_[0] = Voice::scratch_memory_[0];
    Voice::scratch_arrays_[1] = Voice::scratch_memory_[1];
    Voice::scratch_buf_ = Voice::scratch_arrays_;
}

void Voice::NoteOn(const float new_frequency) {
    voice_freq_ = new_frequency;
    std::cout << "NoteOn"<<std::endl;
    for (VCO* vco : oscillators_) {
        vco->SetFrequency(voice_freq_);
        vco->SetEnabled(true);
    }
    adsr_->SetTriggerState(true);
    is_triggered_ = true;
}

void Voice::NoteOff() {
    for (VCO* vco : oscillators_) {
       // vco->SetEnabled(false);
    }
    adsr_->SetTriggerState(false);
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

void Voice::SetADSR(ADSR* adsr) {
    adsr_ = adsr;
}

void Voice::OperateOnSignal(float** out, unsigned int num_samples) {
    memset((void*)(Voice::scratch_buf_[0]), 0, FRAMES_PER_BUFFER*sizeof(float));
    memset((void*)(Voice::scratch_buf_[1]), 0, FRAMES_PER_BUFFER*sizeof(float));
    
    for (VCO* osc : oscillators_) {
        osc->SuperimposeNextSamples(Voice::scratch_buf_, num_samples);
    }

    adsr_->OperateOnSignal(Voice::scratch_buf_, num_samples);

    for (int i = 0; i<num_samples; ++i) {
        out[0][i] += Voice::scratch_buf_[0][i];
        out[1][i] += Voice::scratch_buf_[1][i];
    }
}

void Voice::SetAttack(const float knob_value) {
    adsr_->SetAttack(knob_value);
}

void Voice::SetDecay(const float knob_value) {
    adsr_->SetDecay(knob_value);
}

void Voice::SetRelease(const float knob_value) {
    adsr_->SetRelease(knob_value);
}

void Voice::SetSustain(const float knob_value) {
    adsr_->SetSustain(knob_value);
}