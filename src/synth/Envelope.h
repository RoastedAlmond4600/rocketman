#pragma once
const float SILENCE = 0.0001f;
class Envelope {
public:
    float nextValue() {
        //Cheeky first order lpf.
        level = alpha * (level - target) + target;
        if (target + level >= 3.0f) {
            multiplier = decayMult;
            target = sustainMult;
        }
        return level;
    }
    float reset() {
        level = 0.0f;
        target = 0.0f;
        alpha = 0.0f;
    }
    void release() {
        target = 0.0f;
        alpha = releaseMult;
    }
    inline bool isActive() const {
        return level > SILENCE;
    }
    inline bool isAttack() const {
        return target >= 2.0f;
    }
    void attack() {
        //A minimum amount so that isActive returns true.
        level += SILENCE + SILENCE;
        target = 2.0f;
        alpha = attackMult;
    }

    float level;
    float attackMult;
    float decayMult;
    float sustainMult;
    float releaseMult;

private:
    //The amount of gain to be applied to a particular signal
    float alpha; 
    //The tendency for all these first order lpfs we're writing.
    float target;
};
