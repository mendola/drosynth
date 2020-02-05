#include "midievent.hpp"
#include <math.h>

static constexpr int OCTAVE_OFFSET = 4;

MidiEvent::MidiEvent(double timeStamp, std::vector<unsigned char> *message, void *userData) {
    frequency_ = ParseFrequency(message);
    velocity_ = ParseVelocity(message);
    event_type_ = ParseEventType(message);
}

inline float MidiEvent::ParseFrequency(std::vector<unsigned char> *message) {
    float note_idx = (float)message->at(1) - 12.0 * OCTAVE_OFFSET;
    frequency_ = pow(2, note_idx / 12) * 440.0;
}

inline float MidiEvent::ParseVelocity(std::vector<unsigned char> *message) {
    return 127.0 / (float)message->at(2);
}

inline eventtype_t MidiEvent::ParseEventType(std::vector<unsigned char> *message) {
    int code = (int)message->at(0);
    eventtype_t ret;
    switch (code) {
        case 144:
            ret = NOTE_ON;
            break;
        case 128:
            ret = NOTE_OFF;
            break;
        default:
            ret = UNKNOWN;
    }
    return ret;
}
