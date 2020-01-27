#include "waveform.hpp"

WaveGenerator::~WaveGenerator() {
    delete[] waveform_;
}

