
#include <cassert>
#include <cmath>
#include "oscillator.hpp"
#include "trianglewave.hpp"

TriangleWave::TriangleWave(const float sample_rate_hz, const float frequency, const float amplitude) :
        Oscillator(sample_rate_hz, frequency, amplitude) {

}

// TODO delete this and the other computeAts
inline float TriangleWave::computeAt(const uint32_t idx) {
    const int samples_per_cycle = (double)sample_rate_hz_ / ((double)osc_frequency_*(double)detune_factor_);
    return (idx % samples_per_cycle) >= (samples_per_cycle / 2) ? amplitude_ : -amplitude_;
}

inline void TriangleWave::SuperimposeNextSamples(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;
    const float dy_dsample = amplitude_ / period_in_samples_ / 4;
    const uint32_t pos_peak_idx = period_in_samples_ / 4;
    const uint32_t neg_peak_idx =  period_in_samples_ * 3 / 4;
    static float new_sample = 0;
    for (uint32_t i=0; i<num_samples; ++i) {
        if (phase_index_ < pos_peak_idx) {
            new_sample = phase_index_ * dy_dsample;
        } else if (phase_index_ < neg_peak_idx) {
            new_sample = (2. * amplitude_) - ((phase_index_ - pos_peak_idx) * dy_dsample) ;
        } else {
            new_sample = (-2. * amplitude_) + ((phase_index_ - neg_peak_idx) * dy_dsample) ;
        }
        out[0][i] += new_sample;
        out[1][i] += new_sample;
        ++phase_index_;
        if (phase_index_ >= period_in_samples_ ){
            phase_index_ = 0;
        }
    }
}