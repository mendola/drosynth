#pragma once

#include <vector>

#include "voice.hpp"
#include "vca.hpp"
#include "adsr.hpp"
#include "midiinput.hpp"
//#include "simplified_ladder.hpp"
#include "huovilainen_ladder.hpp"
#include "portaudiocpp/PortAudioCpp.hxx"

static constexpr unsigned int NUM_VOICES = 5;

typedef struct note {
	NoteOnEvent key_evt;
	Voice* voice;
} Note;

class Synthesizer {
public:
	Synthesizer();
    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
	void HandleKnobTurn(const unsigned int knob_id, const float knob_value);
	void NoteOff(NoteOffEvent evt);
	void NoteOn(NoteOnEvent evt);

	// Voice modulation 
	void SetAllOscFrequencies(const float new_frequency);
	void SetSingleOscAmplitude(const unsigned int idx, const float amplitude);
    void SetAllAttack(const float knob_value);
    void SetAllDecay(const float knob_value);
    void SetAllSustain(const float knob_value);
    void SetAllRelease(const float knob_value);

	// Amplifier modulation
	void SetGain(const float new_gain);
	void SetMasterVolume(const float new_amplitude);

	// Filter modulation
	void SetFilterCutoff(const float normalized_cutoff);
	void SetFilterResonance(const float knob_value);

private:
	inline void ClearStereoWaveform(float** out, uint32_t num_samples);
	inline void ScaleStereoWaveformVolume(float** out, uint32_t num_samples);
protected:

	MidiInput minilogue;

	std::vector<Note> keys_pressed_;

	std::vector<Voice*> voices_;

	//SimpleLadder* lpf_;
	HuovilainenLadder* lpf_;

	VCA* amplifier_;

	float master_volume_ = 1.0;

	unsigned int total_num_oscillators_ = 0;
};
