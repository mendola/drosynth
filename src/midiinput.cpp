#include <iostream>
#include <cstdlib>
#include <functional>

#include "RtMidi.h"
#include "midiinput.hpp"
#include "midievent.hpp"

static std::string MINILOGUE_NAME = "minilogue:minilogue MIDI 2 20:1";


void midiEventCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
    MidiInput* caller = (MidiInput*)userData;
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ ) {
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    }
    if ( nBytes > 0 ) {
        std::cout << "stamp = " << deltatime << std::endl;
    }

    eventtype_t eventtype = GetMidiEventType(deltatime, message, userData);

    if (eventtype == NOTE_ON
            && caller != NULL
            && caller->midiNoteOnCallback != NULL) {
        NoteOnEvent evt = NoteOnEvent(deltatime, message, userData);
        caller->midiNoteOnCallback(evt.frequency_);
    } else if (eventtype == NOTE_OFF
            && caller != NULL
            && caller->midiNoteOffCallback != NULL) {
        NoteOffEvent evt = NoteOffEvent(deltatime, message, userData);
        caller->midiNoteOffCallback();
    } else if (eventtype == KNOB_TURN
            && caller != NULL
            && caller->knobTurnCallback != NULL) {
        KnobTurnEvent evt = KnobTurnEvent(deltatime, message, userData);
        caller->knobTurnCallback(evt.knob_id_, evt.knob_value_);
    }
}

MidiInput::MidiInput() {
    midi_in_ = new RtMidiIn();
    if (FindAndOpenDevice() < 0) {
        std::cout << "Couldn't find minilogue." << std::endl;
        exit( EXIT_FAILURE );
    }
    midi_in_->setCallback(&midiEventCallback, this);
}

MidiInput::~MidiInput() {
    delete midi_in_;
}

int MidiInput::FindAndOpenDevice() {
    unsigned int num_ports = midi_in_->getPortCount();
    std::cout << "\nThere are " << num_ports << " MIDI input sources available.\n";
    std::string port_name;
    for (unsigned int i=0; i<num_ports; ++i) {
       port_name =  midi_in_->getPortName(i);
       if (MINILOGUE_NAME == port_name) {
           std::cout << "Found minilogue on port " << i << std::endl;
           port_id_ = i;
           midi_in_->openPort(port_id_);
           return 0;
       }
    }
    return -1;
}

// void MidiInput::SetCallbackFunction(callbackfunction func) {
//     external_callback_ = func;
// }


void MidiInput::Start() {
    //midi_in_->setCallback( external_callback_);
    //midi_in_->ignoreTypes(false,false,false);
}

void MidiInput::SetExternalNoteOnCallback(noteoncallback func) {
    midiNoteOnCallback = func;
}

void MidiInput::SetExternalNoteOffCallback(noteoffcallback func) {
    midiNoteOffCallback = func;
}

void MidiInput::SetExternalKnobTurnCallback(knobturncallback func) {
    knobTurnCallback = func;
}
