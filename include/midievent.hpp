#pragma once
#include <vector>
#include <stdint.h>

typedef enum eventtype_ : int{
    UNKNOWN,
    NOTE_ON,
    NOTE_OFF,
    KNOB_TURN,
} eventtype_t;

inline eventtype_t ParseEventType(std::vector<unsigned char> *message);


class MidiEvent {
public:
    MidiEvent();
    MidiEvent(double timeStamp, std::vector<unsigned char> *message, void *userData);
    eventtype_t event_type_;
protected:
    double timestamp_;
    std::vector<unsigned char> *message_;
    void* user_data_;
};


class NoteOnEvent : public MidiEvent {
public:
    NoteOnEvent(double timeStamp, std::vector<unsigned char> *message, void *userData);
    float frequency_;
    float velocity_;
    unsigned int key_id_;
protected:
    inline float ParseFrequency();
    inline float ParseVelocity();
};

class NoteOffEvent : public MidiEvent {
public:
    NoteOffEvent(double timeStamp, std::vector<unsigned char> *message, void *userData);
    float frequency_;
    float velocity_;
    unsigned int key_id_;
protected:
    inline float ParseFrequency();
    inline float ParseVelocity();
};

class KnobTurnEvent : public MidiEvent {
public:
    KnobTurnEvent(double timeStamp, std::vector<unsigned char> *message, void *userData);
    float knob_value_;
    uint8_t knob_id_;
protected:
    inline float ParseKnobValue();
    inline uint8_t ParseKnobId();
};

eventtype_t GetMidiEventType(double timeStamp, std::vector<unsigned char> *message, void *userData); 
