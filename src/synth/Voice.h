#pragma once
#include "oscillators/DFDROscillator.h"
#include "oscillators/BLSawOscillator.h"
#include "Oscillator.h"
struct Voice {
    int note;
    int oscIndex;
    Oscillator osc;
    DFDROscillator dfosc;
    BLSawOscillator sawOsc;
    void reset() {
        note = 0;
        sawOsc.reset(); 
    }
    float renderOsc() {
        return sawOsc.update(); 
    }
};
