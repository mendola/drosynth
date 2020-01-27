#include "waveform.hpp"

static constexpr double MIN_FREQUENCY = 20.0;  // Hz
static constexpr double SAMPLE_RATEB = 44100.0;  // Hz
static constexpr int MAX_WAVEFORM_LENGTH = 1 + static_cast<int>(SAMPLE_RATEB / MIN_FREQUENCY);

WaveGenerator::WaveGenerator() {
    waveform_ = new float[MAX_WAVEFORM_LENGTH];
}


WaveGenerator::~WaveGenerator() {
    delete[] waveform_;
}

