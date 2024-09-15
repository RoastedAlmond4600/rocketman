#pragma once
#include <math.h>
const float TWO_PI = 6.283185307f;
//a direct form digital resonator implementation of a sin wave (subject to change. Maybe not.)
class DFDROscillator {
    public:
        float phase;
        float inc;
        float amplitude;
        void reset() {
            sin0 = amplitude * std::sin(phase * TWO_PI);
            sin1 = amplitude * std::sin((phase - inc) * TWO_PI);
            dsin = 2.f * std::cos(inc * TWO_PI);
        }
        float update() {
            float sinx = dsin * sin0 - sin1;
            sin1 = sin0;
            sin0 = sinx;
            return sinx; 
        }
    private: 
        float sin0;
        float sin1;
        float dsin;
};
