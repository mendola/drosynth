#pragma once
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <functional>

//typedef void (*callbackfunction) (double, std::vector< unsigned char >*, void *);
typedef std::function<void(float)> noteoncallback;
typedef std::function<void()> noteoffcallback;

class MidiInput {
public:
    MidiInput();
    ~MidiInput();
    void Start();
    //void SetCallbackFunction(callbackfunction func);
    void SetExternalNoteOnCallback(noteoncallback func);
    void SetExternalNoteOffCallback(noteoffcallback func);
    //callbackfunction external_callback_ = NULL;
    noteoncallback midiNoteOnCallback = NULL;
    noteoffcallback midiNoteOffCallback = NULL;
private:
    int FindAndOpenDevice();
    void defaultCallback( double deltatime, std::vector< unsigned char > *message, void *userData );
    RtMidiIn* midi_in_ = NULL;
    unsigned int port_id_ = 0;
};
