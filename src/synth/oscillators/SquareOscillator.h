#pragma once
#include "WaveBase.h"
#include <iostream>
class SquareOscillator : public WaveBase {
    public:
        float frequency;
        float sampleRate;
        float inc;
        float phaseBL;
        float amplitude;
        float m; 
        float nyquist;
        void reset() override {
           phaseBL = -0.5f; 
           m = 4.f / TWO_PI;
           nyquist = sampleRate / 2.f;
        }
        float update() override {
            phaseBL += inc; 
            if (phaseBL >= 1.f) {
                phaseBL -= 1.f;
            }
            float output = 0.f;
            float i = 1.f;
            float h = frequency;
            while (h < nyquist) {
                output += m * std::sin(TWO_PI * phaseBL * i) / i;
                i += 2.f;
                m = -m;
                h += frequency;
            }
            return amplitude * output;
        }
    private:
};
