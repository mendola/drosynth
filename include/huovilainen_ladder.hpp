#pragma once
#include "filter.hpp"

class HuovilainenLadder : public Filter {
public:
    HuovilainenLadder(float sampleRate);
    void SetCutoffFreq(const float freq);
    void SetResonance(const float resonance);
    virtual void OperateOnSignal(float** out, unsigned int num_samples) final;
private:
	double stage[4] = {0,0,0,0};
	double stageTanh[3] = {0,0,0};
	double delay[6] = {0,0,0,0,0,0};
	double thermal;
	double tune;
	double acr;
	double resQuad = 0;
    double resonance;
    double cutoff;
    double sampleRate_;
};
