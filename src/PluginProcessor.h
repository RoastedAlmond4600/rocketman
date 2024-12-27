#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "synth/RocketmanSynth.h"

namespace ParameterID {
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
    //Oscillator Positions
    PARAMETER_ID(osc1Pos);
    PARAMETER_ID(osc2Pos);
    PARAMETER_ID(osc3Pos);
    //Oscillator Volumes
    PARAMETER_ID(osc1Vol);
    PARAMETER_ID(osc2Vol);
    PARAMETER_ID(osc3Vol);
    //Oscillator Transpose
    PARAMETER_ID(globalTrans);
    PARAMETER_ID(osc1Trans);
    PARAMETER_ID(osc2Trans);
    PARAMETER_ID(osc3Trans);
    //Envelope Parameters
    PARAMETER_ID(ampAttack);
    PARAMETER_ID(ampDecay);
    PARAMETER_ID(ampSustain);
    PARAMETER_ID(ampRelease);
    #undef PARAMETER_ID
}

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor,
                                        public juce::ValueTree::Listener
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState apvts {*this, NULL, "apvts", createParameterLayout()};

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
    RocketmanSynth synth;
    void splitBufferEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer);
    void handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2);
    void render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset);
    void update();
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::atomic<bool> parameterChanged { true };
    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) {
        parameterChanged.store(true);
    }
    juce::AudioParameterInt* osc1PosParam;
    juce::AudioParameterInt* osc2PosParam;
    juce::AudioParameterInt* osc3PosParam;
    //Oscilalotor Volume Parameters
    juce::AudioParameterFloat* osc1VolParam;
    juce::AudioParameterFloat* osc2VolParam;
    juce::AudioParameterFloat* osc3VolParam;
    //Oscillator Transport Parameters
    juce::AudioParameterInt* globalTransParam;
    juce::AudioParameterInt* osc1TransParam;
    juce::AudioParameterInt* osc2TransParam;
    juce::AudioParameterInt* osc3TransParam;
    //Amp Envelope Parameters
    juce::AudioParameterFloat* ampAttackParam;
    juce::AudioParameterFloat* ampDecayParam;
    juce::AudioParameterFloat* ampSustainParam;
    juce::AudioParameterFloat* ampReleaseParam;
};
