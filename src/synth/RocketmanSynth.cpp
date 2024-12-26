#include "RocketmanSynth.h"
#include "../utils/ProtectEars.h"

void RocketmanSynth::allocateResources(double _sampleRate, int _samplesPerBlock) {
    sampleRate = static_cast<float>(_sampleRate);
    samplesPerBlock = _samplesPerBlock;
}

void RocketmanSynth::deallocateResources() {
    reset();
}

void RocketmanSynth::reset() {
//Set a bunch of things to 0.
    noiseOsc.reset();
    updateOsc();
    voice.reset();
}

void RocketmanSynth::render(float** buffer, int sampleCount) {
    float* leftBuffer = buffer[0];
    float* rightBuffer = buffer[1];
    float outputLeft = 0.0f;
    float outputRight = 0.0f;
    for (int sample = 0; sample < sampleCount; ++sample) {
        //If a voice is active.
        if (voice.note > 0) {
            //If the oscillator value changes, update the oscillator
            updateOsc();
            float oscValue;
            if (globalTranspose) {
                oscValue = voice.renderOsc(globalTranspose);
            }
            else {
                oscValue = voice.renderOsc(1.f);
            }
            outputLeft = oscValue;
            outputRight = oscValue;
            if (rightBuffer != nullptr) {
                leftBuffer[sample] = outputLeft;
                rightBuffer[sample] = outputRight;
            }
            else {
                leftBuffer[sample] = (outputLeft + outputRight) * 0.5f;
            }
        }
        //protectEars(leftBuffer, sampleCount);
        //protectEars(rightBuffer, sampleCount);
    }
}

void RocketmanSynth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2) {
    //Check the upper four bits of the status byte
    switch (data0 & 0xF0) {
        case 0x80:
            noteOff(data1 & 0x7F);
            break;
        case 0x90: {
            int note = data1 & 0x7F;
            int velocity = data2 & 0x7f;
            if (velocity > 0) {
                noteOn(note, velocity);
            }
            else {
                noteOff(note);
            } 
            break;
        }
    }
}

void RocketmanSynth::noteOn(int note, int velocity) {
    float frequency = 440.f * std::exp2((float)(note - 69 + globalTranspose) / 12.f);
    voice.osc_common->amplitude = (velocity / 127.f) * 0.5f;
    voice.osc_common->inc = frequency / sampleRate;
    std::printf("note_on increment: %f\n", voice.osc_common->inc);
    voice.osc_common->frequency = frequency;
    voice.osc_common->sampleRate = sampleRate;
    voice.reset(); 
    voice.note = note;
}

void RocketmanSynth::noteOff(int note) {
    //Set a release or something. 
    if (voice.note == note) {
        voice.reset();
        voice.release();
    }
}
