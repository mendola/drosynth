#pragma once
#include <vector>
typedef enum eventtype_ : int{
    UNKNOWN,
    NOTE_ON,
    NOTE_OFF,
} eventtype_t;

class MidiEvent {
public:
    MidiEvent(double timeStamp, std::vector<unsigned char> *message, void *userData);
    float frequency_;
    float velocity_;
    eventtype_t event_type_;
private:
    inline float ParseFrequency(std::vector<unsigned char> *message);
    inline float ParseVelocity(std::vector<unsigned char> *message);
    inline eventtype_t ParseEventType(std::vector<unsigned char> *message);
};
