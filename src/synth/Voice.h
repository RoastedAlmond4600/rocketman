#pragma once
#include "oscillators/DFDROscillator.h"
#include "oscillators/BLSawOscillator.h"
#include "Oscillator.h"
struct Voice {
    int note;
    int velocity;
    struct oscillator_Common* osc_common;
    Oscillator oscArray[3];
    Voice() {
        osc_common = (struct oscillator_Common*)malloc(sizeof(struct oscillator_Common));
    }
    void reset() {
        note = 0;
        //Set common values amongst the oscillators
        for (unsigned int i = 0; i < 3; ++i) {
            oscArray[i].amplitude = osc_common->amplitude;
            oscArray[i].inc = osc_common->inc;
            oscArray[i].frequency = osc_common->frequency;
            oscArray[i].sampleRate = osc_common->sampleRate;
            oscArray[i].reset();
        }
    }
    float renderOsc() {
        float output = 0.f;
        for (unsigned int i = 0; i < 3; ++i) {
            output += oscArray[i].update() * 0.707f;
        }
        return output; 
    }
};
