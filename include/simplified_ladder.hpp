#pragma once
#include "filter.hpp"

class SimpleLadder : public Filter {
public:
    SimpleLadder();
    void SetCutoffFreq(const float freq);
    virtual void OperateOnSignal(float** out, unsigned int num_samples) final;
private:
    float output_;
    float last_stage_;
    float stage_[4];
    float stage_Z1_[4];
    float stage_tanh_[3];

    float input_;
    float h_;
    float h0_;
    float gain_;
    float gain_compensation_;

    bool locked_ = false;
};