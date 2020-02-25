#pragma once

class ADSR {
public:
    ADSR();
    void SetTriggerState(const bool trig);
    void OperateOnSignal(float** out, unsigned int num_samples);
    void SetAttack(const float norm_attack);
    void SetDecay(const float norm_decay);
    void SetSustain(const float norm_sustain);
    void SetRelease(const float norm_release);

    inline float GetNextOutput();
private:
    // Settings
    unsigned int attack_time_samples_= 0;
    unsigned int decay_time_samples_ = 0;
    float sustain_level_norm_ = 0;
    unsigned int release_time_samples_ = 44100;
    float norm_attack_;
    float norm_decay_;
    float norm_sustain_;
    float norm_release_;


    // Internal State
    bool is_triggered_ = false;
    unsigned int curr_sample_idx_ = 0;

    float attack_slope_ = 0.0;
    float decay_slope_ = 0.0;
    float release_slope_ = -sustain_level_norm_ / release_time_samples_;

    float curr_output_ = 0;
};