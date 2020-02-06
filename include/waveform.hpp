#pragma once

#include <vector>

#include "oscillator.hpp"
#include "portaudiocpp/PortAudioCpp.hxx"

class WaveGenerator {
public:
	void AddOscillator(Oscillator* osc); 
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
	void SetAllOscFrequencies(const float new_frequency);
	void SetMasterVolume(const float new_amplitude);
	void HandleKnobTurn(const unsigned int knob_id, const float knob_value);
private:
	inline void ClearStereoWaveform(float** out, uint32_t num_samples);
	inline void ScaleStereoWaveformVolume(float** out, uint32_t num_samples);
protected:
	std::vector<Oscillator*> oscillators_;
	float master_volume_ = 1.0;
};
