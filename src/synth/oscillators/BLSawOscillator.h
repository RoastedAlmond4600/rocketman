#pragma once
#include "WaveBase.h"
class BLSawOscillator : public WaveBase {
    public:
        float frequency;
        float sampleRate;
        float inc;
        float phaseBL;
        float amplitude;
        void reset() override {
           phaseBL = -0.5f; 
        }
        float update() override {
            phaseBL += inc; 
            if (phaseBL >= 1.f) {
                phaseBL -= 1.f;
            }
            float output = 0.f;
            float nyquist = sampleRate / 2.f;
            float m = 4.f / TWO_PI;
            float i = 1.f;
            float h = frequency;
            while (h < nyquist) {
                output += m * std::sin(TWO_PI * phaseBL * i) / i;
                i += 1.f;
                m = -m;
                h += frequency;
            }
            return amplitude * output;
        }
    private:
};
