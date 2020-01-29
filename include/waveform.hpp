#pragma once

#include <vector>

#include "oscillator.hpp"
#include "portaudiocpp/PortAudioCpp.hxx"

class WaveGenerator {
public:
	WaveGenerator();
	~WaveGenerator();
	void AddOscillator(Oscillator* osc); 
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
	void SetAllOscFrequencies(const float new_frequency);

private:
	void RenderAndSuperimpose(Oscillator* osc);
	void Render();
	void SetMasterFrequency(const float new_frequency);


protected:
	float *waveform_;
	float master_frequency_;
	size_t waveform_size_;
	int left_phase_idx_;
	int right_phase_idx_;
	std::vector<Oscillator*> oscillators_;
};
