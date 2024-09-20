#pragma once
#include <math.h>
#include "WaveBase.h"
class TriangleOscillator : public WaveBase {
    public:
        float inc;
        float phase;
        float amplitude;
        void reset() override {
            phase = 0.f;
            attenuationFactor = 0.63f;
        }
        float update() override {
            phase += inc;
            if (phase >= 1.f) {
                phase -=1.f;
            }
            return amplitude * std::asin(std::cos(phase)) * attenuationFactor;
        }
    private:
        float attenuationFactor;
};
