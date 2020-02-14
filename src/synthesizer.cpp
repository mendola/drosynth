#include "synthesizer.hpp"
#include <cassert>
#include <iostream>

#include "SineWave.hpp"
#include "squarewave.hpp"
#include "trianglewave.hpp"
#include "midievent.hpp"
#include "midiinput.hpp"

static constexpr double MIN_FREQUENCY = 20.0;  // Hz
static constexpr double SAMPLE_RATEB = 44100.0;  // Hz
static constexpr int MAX_WAVEFORM_LENGTH = 1 + static_cast<int>(SAMPLE_RATEB / MIN_FREQUENCY);

Synthesizer::Synthesizer() {
    for (unsigned int i = 0; i < NUM_VOICES; ++i) {
        Voice *voice = new Voice();
        Sinewave *sw = new Sinewave(SAMPLE_RATEB, 100, 0.125);
        Squarewave *sq = new Squarewave(SAMPLE_RATEB, 100, 0.125);
        TriangleWave *tr = new TriangleWave(SAMPLE_RATEB, 100, 0.125);
        ADSR *adsr = new ADSR();
        voice->AddVCO(sw);
        voice->AddVCO(sq);
        voice->AddVCO(tr);
        voice->SetADSR(adsr);
        voices_.push_back(voice);
        total_num_oscillators_ += 3;
    }
	//VCA *vca = new VCA();
    SimpleLadder *lpf = new SimpleLadder();
    lpf_ = lpf;

    minilogue.Start();
    minilogue.SetExternalNoteOnCallback((noteoncallback)std::bind(&Synthesizer::NoteOn,
                                                                    this,
                                                                    std::placeholders::_1));

    minilogue.SetExternalNoteOffCallback((noteoffcallback)std::bind(&Synthesizer::NoteOff,
                                                                    this,
                                                                    std::placeholders::_1));

    minilogue.SetExternalKnobTurnCallback((knobturncallback)std::bind(&Synthesizer::HandleKnobTurn,
                                                                    this,
                                                                    std::placeholders::_1,
                                                                    std::placeholders::_2));

}

int Synthesizer::generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    ClearStereoWaveform(out, framesPerBuffer);
    static int i = 0;
    //std::cout << ++i<< " " << statusFlags << std::endl;
    for (Voice* voice : voices_) {
        voice->OperateOnSignal(out, framesPerBuffer);
    }
    
    ScaleStereoWaveformVolume(out, framesPerBuffer);

    lpf_->OperateOnSignal(out, framesPerBuffer);
    return paContinue;
}

void Synthesizer::NoteOn(NoteOnEvent evt) {

    if (keys_pressed_.size() >= NUM_VOICES) {
        // Gotta get rid of oldest key press
        Voice* voice_to_silence = keys_pressed_.at(0).voice;
        voice_to_silence->NoteOff();
        keys_pressed_.erase(keys_pressed_.begin());
    }

    for (Voice* voice : voices_) {
        if (!voice->IsOn()) {
            Note note = {evt, voice};
            keys_pressed_.emplace_back(note);
            voice->NoteOn(evt.frequency_);
            break;
        }
    }
}

void Synthesizer::NoteOff(NoteOffEvent evt) {
    unsigned int key_id = evt.key_id_;
    for (unsigned int i = 0; i < keys_pressed_.size(); ++i) {
        Note existing_note = keys_pressed_.at(i);
        if (key_id == existing_note.key_evt.key_id_) {
            existing_note.voice->NoteOff();
            keys_pressed_.erase(keys_pressed_.begin() + i);
            break;
        }
    }
}

inline void Synthesizer::ClearStereoWaveform(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;

    for (size_t i=0; i<num_samples; ++i) {
        out[0][i] = 0;
        out[1][i] = 0;
    }
}

inline void Synthesizer::ScaleStereoWaveformVolume(float** out, uint32_t num_samples) {
    if (out == NULL || *out==NULL) return;

    const float net_scale = master_volume_ / total_num_oscillators_;

    for (size_t i=0; i<num_samples; ++i) {
        out[0][i] *= net_scale;
        out[1][i] *= net_scale;
    }
}

void Synthesizer::SetMasterVolume(const float new_amplitude) {
    master_volume_ = new_amplitude;
}

void Synthesizer::SetSingleOscAmplitude(const unsigned int idx, const float amplitude) {
    for (Voice* voice : voices_) {
        voice->SetOneOscillatorAmplitude(idx, amplitude);
    }
}

void Synthesizer::HandleKnobTurn(const unsigned int knob_id, const float knob_value) {
    switch (knob_id) {
        case 34:
            SetMasterVolume(knob_value);
            break;
        case 35:
            SetSingleOscAmplitude(0, knob_value);
            break;
        case 37:
            SetSingleOscAmplitude(1, knob_value);
            break;
        case 40:
            SetSingleOscAmplitude(2, knob_value);
            break;
        case 43:
            SetFilterCutoff(knob_value);
            break;
        case 16:
            SetAllAttack(knob_value);
            break;
        case 17:
            SetAllDecay(knob_value);
            break;
        case 18:
            SetAllSustain(knob_value);
            break;
        case 19:
            SetAllRelease(knob_value);
            break;
        default:
            break;
    }
}

void Synthesizer::SetFilterCutoff(const float normalized_cutoff) {
    lpf_->SetCutoffFreq(normalized_cutoff * 15000.0);
}

void Synthesizer::SetAllAttack(const float knob_value) {
    for (Voice* voice : voices_) {
        voice->SetAttack(knob_value);
    }
}

void Synthesizer::SetAllDecay(const float knob_value) {
    for (Voice* voice : voices_) {
        voice->SetDecay(knob_value);
    }
}

void Synthesizer::SetAllSustain(const float knob_value) {
    for (Voice* voice : voices_) {
        voice->SetSustain(knob_value);
    }
}

void Synthesizer::SetAllRelease(const float knob_value) {
    for (Voice* voice : voices_) {
        voice->SetRelease(knob_value);
    }
}
