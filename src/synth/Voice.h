#pragma once
#include "oscillators/DFDROscillator.h"
#include "oscillators/BLSawOscillator.h"
#include "Oscillator.h"
struct Voice {
    int note;
    Oscillator osc;
    void reset() {
        note = 0;
        osc.reset();
    }
    float renderOsc() {
        return osc.update();
    }
};
