#include <cassert>
#include <cmath>
#include "oscillator.hpp"
#include "SineWave.hpp"

static constexpr double PI = 3.14159265;

Sinewave::Sinewave(const float sample_rate_hz, const float frequency, const float amplitude) :
        Oscillator(sample_rate_hz, frequency, amplitude) {

}

inline float Sinewave::computeAt(const uint32_t idx) {
    return amplitude_ * (float)sin(((double)idx/(double)sample_rate_hz_*(double)osc_frequency_*(double)detune_factor_)*PI*2.);
}

void Sinewave::SuperimposeNextSamples(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;

    for (uint32_t i=0; i<num_samples; ++i) {
        float new_sample = amplitude_ * (float)sin(((double)continuous_sample_index_/(double)sample_rate_hz_*(double)osc_frequency_*(double)detune_factor_)
                                                    *PI*2.);
        out[0][i] += new_sample;
        out[1][i] += new_sample;
        continuous_sample_index_ += 1.0;
        if (continuous_sample_index_ >= period_in_fractional_samples_) {
            continuous_sample_index_ -= period_in_fractional_samples_;
        }
    }
}
