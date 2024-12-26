#pragma once

class WaveBase {
    public:
        virtual void reset() = 0;
        virtual float update() = 0;
        float inc = 0;
};
