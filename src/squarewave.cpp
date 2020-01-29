#include <cassert>
#include <cmath>
#include "oscillator.hpp"
#include "squarewave.hpp"


Squarewave::Squarewave(const float sample_rate_hz, const float frequency, const float amplitude) :
        Oscillator(sample_rate_hz, frequency, amplitude) {

}

inline float Squarewave::computeAt(const uint32_t idx) {
    const int samples_per_cycle = (double)sample_rate_hz_ / ((double)osc_frequency_*(double)detune_factor_);
    return (idx % samples_per_cycle) >= (samples_per_cycle / 2) ? amplitude_ : -amplitude_;
}
