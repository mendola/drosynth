#include "vca.hpp"
#include <cstddef>
void VCA::SetGainIn(const float gain) {
    gain_ = gain;
}

void VCA::SetVolume(const float volume) {
    volume_ = volume;
}


void VCA::OperateOnSignal(float** out, unsigned int num_samples) {
    if (out == NULL || *out==NULL) return;

    const float net_gain = gain_ * volume_;

    for (unsigned int i=0; i<num_samples; ++i) {
        out[0][i] *= net_gain;
        out[1][i] *= net_gain;
    }
}
