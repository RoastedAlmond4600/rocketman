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
    //Oscillator Parameters
    int osc1Index;
    int osc2Index;
    int osc3Index;
    int osc1Volume;
    int osc2Volume;
    int osc3Volume;

private:
    double sampleRate;
    int samplesPerBlock;
    NoiseOscillator noiseOsc;
    //MIDI stuff
    void noteOn(int note, int velocity);
    void noteOff(int note);
    //God awful
    Voice voice;
    const inline void updateOsc() {
        voice.oscArray[0].waveIndex = osc1Index;
        voice.oscArray[1].waveIndex = osc2Index;
        voice.oscArray[2].waveIndex = osc3Index;
        voice.oscArray[0].amplitude *= osc1Volume;
        voice.oscArray[1].amplitude *= osc2Volume;
        voice.oscArray[2].amplitude *= osc3Volume;
    }
};

