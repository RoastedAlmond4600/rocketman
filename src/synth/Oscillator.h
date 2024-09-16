#pragma once
#define NUMWAVEPOS 2
#include "oscillators/WaveBase.h"
#include "oscillators/DFDROscillator.h"
#include "oscillators/BLSawOscillator.h"
class Oscillator {
    public:
        float amplitude;
        float inc;
        float phase;
        float sampleRate;
        float frequency;
        float sampleIndex;
        int waveIndex;
        WaveBase* waveArray[NUMWAVEPOS] = {&sineWave, &sawWave};
        void reset() {
            //Initialise DFDR
            sineWave.amplitude = amplitude;
            sineWave.inc = inc;
            //Initialise BLSaw
            sawWave.amplitude = amplitude;
            sawWave.inc = inc;
            sawWave.frequency = frequency;
            sawWave.sampleRate = sampleRate;
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
        BLSawOscillator sawWave;
};
