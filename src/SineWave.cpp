#include <cassert>
#include <cmath>
#include "oscillator.hpp"
#include "SineWave.hpp"

static constexpr double PI = 3.14159265;

Sinewave::Sinewave(const float sample_rate_hz, const float frequency, const float amplitude) :
        Oscillator(sample_rate_hz, frequency, amplitude) {

}

inline float Sinewave::computeAt(const uint32_t idx) {
    return amplitude_ * (float)sin(((double)idx/(double)sample_rate_hz_*(double)osc_frequency_)*PI*2.);
}
