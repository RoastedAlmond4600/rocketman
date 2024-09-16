#pragma once
#include "oscillators/NoiseOscillator.h"
#include "Voice.h"
#include <JuceHeader.h>

class RocketmanSynth {
public:
    RocketmanSynth();
    //Overhead
    void allocateResources(double _sampleRate, int samplesPerBlock);
    void deallocateResources();
    void reset();
    void render(float** buffer, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);


    //Volume Parameters
    float noiseMix;
    float oscMix;
    float velocitySens;
    bool ignoreVelocity;

    //Pitch Parameters
    float oscTune;
    float oscFine;
    float detune;
    float tune;
    float pitchBend;
    float bendRange;
    float stereo;

    //Envelope Parameters;
    float envAttack;
    float envDecay;
    float envSustain;
    float envRelease;
    int osc1Index;
    int osc2Index;
    int osc3Index;

private:
    double sampleRate;
    int samplesPerBlock;
    NoiseOscillator noiseOsc;
    //MIDI stuff
    void noteOn(int note, int velocity);
    void noteOff(int note);
    //God awful
    Voice voice;

};

