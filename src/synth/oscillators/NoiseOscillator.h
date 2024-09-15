#pragma once

class NoiseOscillator {
    public:
        void reset() {
            noiseSeed = 22222;
            amplitude = 1.f;
        }
        float update() {
            noiseSeed = noiseSeed * 196314165 + 907633515;
            int temp = int(noiseSeed >> 7) - 16777216;
            return (static_cast<float>(temp) / 16777216.0f); 
        }
    private:
        unsigned int noiseSeed;
        float amplitude;
};
