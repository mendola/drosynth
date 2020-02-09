#pragma once

class Filter {
public:
    void SetCutoffFreq(const float freq);
    void SetResonance(const float resonance);
    virtual void OperateOnSignal(float** out, unsigned int num_samples) = 0;
protected:
    float cutoff_frequency_;
    float resonance_;
};