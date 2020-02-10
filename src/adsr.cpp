#include "adsr.hpp"

#include <iostream>
static constexpr float MAX_ATTACK_SAMPLES = 88000;
static constexpr float MAX_DECAY_SAMPLES = 88000;
static constexpr float MAX_RELEASE_SAMPLES = 88000;

ADSR::ADSR() {
    SetAttack(0.5);
    SetDecay(0.0);
    SetSustain(1.0);
    SetRelease(0.0);
}

void ADSR::SetTriggerState(const bool trig) {
    is_triggered_ = trig;
    if (trig) {
        curr_sample_idx_ = 0;
        curr_output_ = 0;
    } else {
        std::cout<<release_slope_<< std::endl;
    }
}

void ADSR::OperateOnSignal(float** out, unsigned int num_samples) {
    unsigned int attack_time_samples= attack_time_samples_;
    unsigned int decay_time_samples = decay_time_samples_;
    float sustain_level_norm = sustain_level_norm_;
    unsigned int release_time_samples = release_time_samples_;
    float attack_slope = attack_slope_;
    float decay_slope = decay_slope_;
    float release_slope = release_slope_;


    float scale = 0.0;
    for (int i=0; i<num_samples; ++i) {
        if (is_triggered_) {
            if (curr_sample_idx_ < attack_time_samples) {
                curr_output_ += attack_slope;
            } else if (curr_sample_idx_ == attack_time_samples) {
                curr_output_ = 1.0;
            } else if (curr_sample_idx_ < attack_time_samples + decay_time_samples) {
                curr_output_ += decay_slope;
            } else {
                curr_output_ = sustain_level_norm;
            }
            ++curr_sample_idx_;
        } else {
            curr_output_ += release_slope_;
            if (curr_output_ < 0.0) {
                curr_output_ = 0.0;
                curr_sample_idx_ = 0;
            } else {
                ++curr_sample_idx_;
            }
         //   std::cout << curr_output_ <<std::endl;
        }
        //std::cout << curr_output_ << std::endl;
        out[0][i] *= curr_output_;
        out[1][i] *= curr_output_;

    }
}

void ADSR::SetAttack(const float norm_attack) {
    attack_time_samples_ = static_cast<unsigned int>(norm_attack * MAX_ATTACK_SAMPLES);
    attack_slope_ = 1.0 / attack_time_samples_;
}

void ADSR::SetDecay(const float norm_decay) {
    decay_time_samples_ = static_cast<unsigned int>(norm_decay * MAX_DECAY_SAMPLES);
    decay_slope_ = -(1.0 - sustain_level_norm_)  / decay_time_samples_;
}

void ADSR::SetSustain(const float norm_sustain) {
    sustain_level_norm_ = norm_sustain;
    SetDecay(decay_time_samples_);
    SetRelease(release_time_samples_);
}

void ADSR::SetRelease(const float norm_release) {
    release_time_samples_ = static_cast<unsigned int>(norm_release * MAX_RELEASE_SAMPLES);
    release_slope_ = -sustain_level_norm_ / static_cast<float>(release_time_samples_); 
    //std::cout << "Release slope " << release_slope_ << std::endl;
}