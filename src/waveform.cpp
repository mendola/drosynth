#include "waveform.hpp"
#include <cassert>

static constexpr double MIN_FREQUENCY = 20.0;  // Hz
static constexpr double SAMPLE_RATEB = 44100.0;  // Hz
static constexpr int MAX_WAVEFORM_LENGTH = 1 + static_cast<int>(SAMPLE_RATEB / MIN_FREQUENCY);

WaveGenerator::WaveGenerator() {
    waveform_ = new float[MAX_WAVEFORM_LENGTH];
    waveform_size_ = MAX_WAVEFORM_LENGTH;
    left_phase_idx_ = 0;
    left_phase_idx_ = 0;

}


WaveGenerator::~WaveGenerator() {
    delete[] waveform_;
}

int WaveGenerator::generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    for (unsigned int i = 0; i < framesPerBuffer; ++i)
    {
        out[0][i] = waveform_[left_phase_idx_];
        out[1][i] = waveform_[right_phase_idx_];

        left_phase_idx_ += 1;
        if (left_phase_idx_ >= waveform_size_)
            left_phase_idx_ -= waveform_size_;

        right_phase_idx_ += 1;
        if (right_phase_idx_ >= waveform_size_)
            right_phase_idx_ -= waveform_size_;
    }

    return paContinue;
}

void WaveGenerator::AddOscillator(Oscillator* osc) {
    oscillators_.push_back(osc);
    RenderAndSuperimpose(osc);
}

void WaveGenerator::RenderAndSuperimpose(Oscillator* osc) {
    for (size_t i=0; i<waveform_size_; ++i) {
        waveform_[i] += osc->computeAt(i);
    }
}

void WaveGenerator::SetAllOscFrequencies(const float new_frequency) {
    for (Oscillator* osc : oscillators_) {
        osc->SetFrequency(new_frequency);
    }
    SetMasterFrequency(new_frequency);
    Render();
}

void WaveGenerator::SetMasterFrequency(const float new_frequency) {
    master_frequency_ = new_frequency;
    waveform_size_ = SAMPLE_RATEB / master_frequency_;
}

void WaveGenerator::Render() {
    for (size_t i=0; i<waveform_size_; ++i) {
        waveform_[i] = 0;
    }
    for (Oscillator* osc : oscillators_) {
        RenderAndSuperimpose(osc);
    }
}
