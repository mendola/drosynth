#pragma once

#include <vector>

#include "oscillator.hpp"
#include "vca.hpp"
#include "portaudiocpp/PortAudioCpp.hxx"

class Synthesizer {
public:
	void AddVCO(VCO* osc); 
	void AddVCA(VCA* vca);
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
	void HandleKnobTurn(const unsigned int knob_id, const float knob_value);
	void NoteOff();
	void NoteOn(const float new_frequency, const float amplitute);

	// Oscillator modulation 
	void SetAllOscFrequencies(const float new_frequency);
	void SetSingleOscAmplitude(const unsigned int idx, const float amplitude);

	// Amplifier modulation
	void SetGain(const float new_gain);
	void SetMasterVolume(const float new_amplitude);

private:
	inline void ClearStereoWaveform(float** out, uint32_t num_samples);
	inline void ScaleStereoWaveformVolume(float** out, uint32_t num_samples);
protected:

	std::vector<VCO*> oscillators_;
	VCA* amplifier_;

	float master_volume_ = 1.0;
};
