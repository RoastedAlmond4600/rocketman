#pragma once
#include "oscillators/DFDROscillator.h"
#include "oscillators/BLSawOscillator.h"
#include "Oscillator.h"
#include "Envelope.h"
#include <JuceHeader.h>
#define NUMOSC 3
struct Voice {
    int note;
    int velocity;
    struct oscillator_Common* osc_common;
    Envelope env;
    Oscillator oscArray[3];
    Voice() {
        osc_common = (struct oscillator_Common*)malloc(sizeof(struct oscillator_Common));
        osc_common->amplitude = 0.f;
        osc_common->inc = 0.f;
        osc_common->frequency = 0.f;
        osc_common->sampleRate = 0.f;
    }
    void reset() {
        note = 0;
        env.reset();
        //Set common values amongst the oscillators
        for (unsigned int i = 0; i < NUMOSC; ++i) {
            oscArray[i].amplitude *= osc_common->amplitude;
            oscArray[i].inc = osc_common->inc;
            oscArray[i].frequency = osc_common->frequency;
            oscArray[i].sampleRate = osc_common->sampleRate;
            oscArray[i].reset();
        }
    }
    float renderOsc(float inc_factor) {
        float output = 0.f;
        float envelope = env.nextValue();
        for (unsigned int i = 0; i < NUMOSC; ++i) {
            output += oscArray[i].update() * oscArray[i].amplitude;
        }
        //return output * envelope;
        return output; 
    }
    void release() {
        env.release();
    }
};
