#pragma once

#include "portaudiocpp/PortAudioCpp.hxx"

class WaveGenerator {
public:
	WaveGenerator();
	~WaveGenerator();
    virtual int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags) = 0;
protected:
	float *waveform_;
	size_t waveform_size_;
	int left_phase_idx_;
	int right_phase_idx_;
};
