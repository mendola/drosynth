#include "midievent.hpp"
#include <math.h>

static constexpr int OCTAVE_OFFSET = 4;

/************ MidiEvent **************************/
MidiEvent::MidiEvent(double timeStamp, std::vector<unsigned char> *message, void *userData) {
    timestamp_ = timeStamp;
    message_ = message;
    user_data_ = userData;
    event_type_ = UNKNOWN;
}


/************ NoteOnEvent **************************/
NoteOnEvent::NoteOnEvent(double timeStamp, std::vector<unsigned char> *message, void *userData)
    : MidiEvent(timeStamp, message, userData) {
    frequency_ = ParseFrequency();
    velocity_ = ParseVelocity();
    event_type_ = NOTE_ON;
    key_id_ = (float)message_->at(1) - 12.0 * OCTAVE_OFFSET;
}

inline float NoteOnEvent::ParseFrequency() {
    float note_idx = (float)message_->at(1) - 12.0 * OCTAVE_OFFSET;
    frequency_ = pow(2, note_idx / 12) * 440.0;
}

inline float NoteOnEvent::ParseVelocity() {
    return (float)message_->at(2) / 127.0;
}

/************ NoteOffEvent **************************/
NoteOffEvent::NoteOffEvent(double timeStamp, std::vector<unsigned char> *message, void *userData)
    : MidiEvent(timeStamp, message, userData) {
    frequency_ = ParseFrequency();
    velocity_ = 0;
    event_type_ = NOTE_OFF;
    key_id_ = (float)message_->at(1) - 12.0 * OCTAVE_OFFSET;
}

inline float NoteOffEvent::ParseFrequency() {
    float note_idx = (float)message_->at(1) - 12.0 * OCTAVE_OFFSET;
    frequency_ = pow(2, note_idx / 12) * 440.0;
}


/************ KnobTurnEvent***************************/
KnobTurnEvent::KnobTurnEvent(double timeStamp, std::vector<unsigned char> *message, void *userData)
    : MidiEvent(timeStamp, message, userData) {
    knob_value_ = ParseKnobValue();
    knob_id_ = ParseKnobId();
    event_type_ = KNOB_TURN;
}

inline float KnobTurnEvent::ParseKnobValue() {
    return (float)message_->at(2) / 127.0;
}

inline uint8_t KnobTurnEvent::ParseKnobId() {
    return (uint8_t)message_->at(1);
}

/***************  Non-Class Functions *******************/
eventtype_t GetMidiEventType(double timeStamp, std::vector<unsigned char> *message, void *userData) {
    int code = (int)message->at(0);
    switch (code) {
        case 144:
            return NOTE_ON;
        case 128:
            return NOTE_OFF;
        case 176:
            return KNOB_TURN;
        default:
            return UNKNOWN;
    }
}
