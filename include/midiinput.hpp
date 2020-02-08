#pragma once
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include "midievent.hpp"
#include <functional>

//typedef void (*callbackfunction) (double, std::vector< unsigned char >*, void *);
typedef std::function<void(NoteOnEvent)> noteoncallback;
typedef std::function<void(NoteOffEvent)> noteoffcallback;
typedef std::function<void(unsigned int, float)> knobturncallback;


class KeyEvent {
public:
    KeyEvent (const unsigned int key, float velocity) :
        key_id_(key), velocity_(velocity) {}
    float GetFrequency();
    float velocity_;
    unsigned int key_id_;
};

class MidiInput {
public:
    MidiInput();
    ~MidiInput();
    void Start();
    //void SetCallbackFunction(callbackfunction func);
    void SetExternalNoteOnCallback(noteoncallback func);
    void SetExternalNoteOffCallback(noteoffcallback func);
    void SetExternalKnobTurnCallback(knobturncallback func);
    //callbackfunction external_callback_ = NULL;
    noteoncallback midiNoteOnCallback = NULL;
    noteoffcallback midiNoteOffCallback = NULL;
    knobturncallback knobTurnCallback = NULL;

private:
    int FindAndOpenDevice();
    void defaultCallback( double deltatime, std::vector< unsigned char > *message, void *userData );
    RtMidiIn* midi_in_ = NULL;
    unsigned int port_id_ = 0;
};
