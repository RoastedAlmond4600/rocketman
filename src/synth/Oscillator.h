#pragma once
class Oscillator {
    public:
        //An array of reset function pointers
        //an Array of update function pointers
        float amplitude;
        float inc;
        float phase;
        float sampleRate;
        float frequency;
        float sampleIndex;
        void reset() {
            frequency = 0.f;
            amplitude = 0.5f;
            sampleRate = 44100.f;
            sin0 = amplitude * std::sin(phase * TWO_PI);
            sin1 = amplitude * std::sin((phase - inc) * TWO_PI);
            dsin = 2.f * std::cos(inc * TWO_PI);
        }
        float update() {
            float sinx = dsin * sin0 - sin1;
            sin1 = sin0;
            sin0 = sinx;
           return sinx * amplitude; 
        }
    private:
        float sin0;
        float sin1;
        float dsin;
};
