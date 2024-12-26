#pragma once
#define NUMWAVEPOS 4
#include "oscillators/WaveBase.h"
#include "oscillators/DFDROscillator.h"
#include "oscillators/TriangleOscillator.h"
#include "oscillators/BLSawOscillator.h"
#include "oscillators/SquareOscillator.h"

struct oscillator_Common {
    float amplitude;
    float inc;
    float frequency;
    float sampleRate;
};

class Oscillator {
    public:
        float amplitude;
        float inc;
        float phase;
        float sampleRate;
        float frequency;
        float sampleIndex;
        int waveIndex;
        struct oscillator_Common* osc_common;
        WaveBase* waveArray[NUMWAVEPOS] = {&sineWave, &triangleWave, &sawWave, &squareWave};
        void reset() {
            //Initialise DFDR
            sineWave.amplitude = amplitude;
            sineWave.inc = inc;
            //Initialise Triangle
            triangleWave.inc = inc;
            triangleWave.amplitude = amplitude;
            //Initialise BLSaw
            sawWave.amplitude = amplitude;
            sawWave.inc = inc;
            sawWave.frequency = frequency;
            sawWave.sampleRate = sampleRate;
            //Initialise Square 
            squareWave.amplitude = amplitude;
            squareWave.inc = inc;
            squareWave.frequency = frequency;
            squareWave.sampleRate = sampleRate;
            //Reset each waveform
            for (unsigned int i = 0; i < NUMWAVEPOS; ++i) {
                waveArray[i]->reset();
            }
        }
        float update() {
            return waveArray[waveIndex]->update();
        }
    private:
        DFDROscillator sineWave;
        TriangleOscillator triangleWave;
        BLSawOscillator sawWave;
        SquareOscillator squareWave;
};
