#pragma once

class VCA {
public:
    void SetGainIn(const float gain);
    void SetVolume(const float volume);
    void OperateOnSignal(float** out, unsigned int num_samples);
private:
    float gain_ = 0;
    float volume_ = 1.0;
};