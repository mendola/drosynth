#pragma once
#include <vector>

#include "adsr.hpp"
#include "oscillator.hpp"

static const unsigned int FRAMES_PER_BUFFER = 64;

class Voice {
public:
    Voice();
    void NoteOn(const float new_frequency);
    void NoteOff();
    void AddVCO(VCO* vco);
    void SetADSR(ADSR* adsr);
    bool IsOn();
    void SetOneOscillatorAmplitude(const unsigned int idx, const float amplitude);
    void OperateOnSignal(float** out, unsigned int num_samples);
    void SetAttack(const float knob_value);
    void SetDecay(const float knob_value);
    void SetSustain(const float knob_value);
    void SetRelease(const float knob_value);
private:
    std::vector<VCO*> oscillators_;

	ADSR* adsr_;

    float voice_freq_;
    bool is_triggered_ = false;

    static float scratch_memory_[2][FRAMES_PER_BUFFER];
    static float* scratch_arrays_[2];
    static float** scratch_buf_;
};