#pragma once

#include <vector>

#include "voice.hpp"
#include "vca.hpp"
#include "midiinput.hpp"
#include "simplified_ladder.hpp"
#include "portaudiocpp/PortAudioCpp.hxx"

static constexpr unsigned int NUM_VOICES = 5;

typedef struct note {
	NoteOnEvent key_evt;
	Voice* voice;
} Note;

class Synthesizer {
public:
	Synthesizer();
	//void AddVCO(VCO* osc); 
	void AddVCA(VCA* vca);
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
	void HandleKnobTurn(const unsigned int knob_id, const float knob_value);
	void NoteOff(NoteOffEvent evt);
	void NoteOn(NoteOnEvent evt);

	// Oscillator modulation 
	void SetAllOscFrequencies(const float new_frequency);
	void SetSingleOscAmplitude(const unsigned int idx, const float amplitude);

	// Amplifier modulation
	void SetGain(const float new_gain);
	void SetMasterVolume(const float new_amplitude);

	// Filter modulation
	void SetFilterCutoff(const float normalized_cutoff);

private:
	inline void ClearStereoWaveform(float** out, uint32_t num_samples);
	inline void ScaleStereoWaveformVolume(float** out, uint32_t num_samples);
protected:

	MidiInput minilogue;

	std::vector<Note> keys_pressed_;

	std::vector<Voice*> voices_;

	VCA* amplifier_;

	SimpleLadder* lpf_;

	float master_volume_ = 1.0;

	unsigned int total_num_oscillators_ = 0;
};
