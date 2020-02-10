#include "simplified_ladder.hpp"
#include <cstring>
#include <math.h>
#include <iostream>
static constexpr float PI = 3.14159265;
#define SNAP_TO_ZERO(n)    if (! (n < -1.0e-8 || n > 1.0e-8)) n = 0;

SimpleLadder::SimpleLadder() {
        // To keep the overall level approximately constant, comp should be set
		// to 0.5 resulting in a 6 dB passband gain decrease at the maximum resonance
		// (compared to a 12 dB decrease in the original Moog model
		gain_compensation_ = 0.2;
		
		memset(stage_, 0, sizeof(stage_));
		memset(stage_Z1_, 0, sizeof(stage_Z1_));
		memset(stage_tanh_, 0, sizeof(stage_tanh_));
		
		SetCutoffFreq(1000.0f);
		SetResonance(0.0f);
}

void SimpleLadder::OperateOnSignal(float** out, unsigned int num_samples) {
    static float min = 0.0;
    static float max = 0.0;
    float g = gain_;
    float h = h_;
    float h0 = h0_;
    for (int i=0; i<num_samples; ++i) {

        for (int stage_idx=0; stage_idx<4; ++stage_idx) {
            if (stage_idx) {
                input_ = stage_[stage_idx-1];
                stage_tanh_[stage_idx-1] = tanh(input_);

                stage_[stage_idx] = (h* stage_Z1_[stage_idx] + h0 * stage_tanh_[stage_idx-1]) + (1.0 - g) * (stage_idx != 3 ? stage_tanh_[stage_idx] : tanh(stage_Z1_[stage_idx]));
            } else {
                // First stage:
                input_ = out[0][i] - ((4.0 * resonance_) * (output_ - gain_compensation_ * out[0][i]));
                stage_[stage_idx] = (h * tanh(input_) + h0 * stage_Z1_[stage_idx]) + (1.0 - g) * stage_tanh_[stage_idx];
            }
            stage_Z1_[stage_idx] = stage_[stage_idx];
        }
        
        output_ = stage_[3];
        SNAP_TO_ZERO(output_);
        out[0][i] = output_;
        out[1][i] = output_;
    }
    if (output_ > max) {
        max = output_;
 //       std::cout << max << std::endl;
    }
}

void SimpleLadder::SetCutoffFreq(const float freq) {
    cutoff_frequency_ = freq;
    std::cout << cutoff_frequency_ << std::endl;
    // Not being oversampled at the moment... * 2 when functional
    float fs2 = 44100.0;
    
    // Normalized cutoff [0, 1] in radians: ((2*pi) * cutoff / samplerate)
    gain_ = (2 * PI) * cutoff_frequency_ / fs2; // feedback coefficient at fs*2 because of doublesampling
    gain_ *= PI / 1.3; // correction factor that allows _cutoff to be supplied Hertz
    // FIR part with gain g
    h_ = gain_ / 1.3;
    h0_ = gain_ * 0.3 / 1.3;
}
