#pragma once
#include <vector>
#include "oscillator.hpp"

class Voice {
public:
    void NoteOn(const float new_frequency);
    void NoteOff();
    void AddVCO(VCO* vco);
    bool IsOn();
    void SetOneOscillatorAmplitude(const unsigned int idx, const float amplitude);
    void OperateOnSignal(float** out, unsigned int num_samples);
private:
    std::vector<VCO*> oscillators_;
    float voice_freq_;
    bool is_triggered_ = false;
};