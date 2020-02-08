#include <cassert>
#include <cmath>
#include "oscillator.hpp"
#include "squarewave.hpp"


Squarewave::Squarewave(const float sample_rate_hz, const float frequency, const float amplitude) :
        VCO(sample_rate_hz, frequency, amplitude) {

}

inline float Squarewave::computeAt(const uint32_t idx) {
    const int samples_per_cycle = (double)sample_rate_hz_ / ((double)osc_frequency_*(double)detune_factor_);
    return (idx % samples_per_cycle) >= (samples_per_cycle / 2) ? v_in_ : -v_in_;
}

inline void Squarewave::SuperimposeNextSamples(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;

    for (uint32_t i=0; i<num_samples; ++i) {
        float new_sample = continuous_sample_index_ <= (period_in_fractional_samples_ / 2) ? v_in_ : -v_in_;

        out[0][i] += new_sample;
        out[1][i] += new_sample;
        continuous_sample_index_ += 1.0;
        if (continuous_sample_index_ >= period_in_fractional_samples_) {
            continuous_sample_index_ -= period_in_fractional_samples_;
        }
    }
}
