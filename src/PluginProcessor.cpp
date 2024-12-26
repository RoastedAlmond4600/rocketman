#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils/CastParameter.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    castParameter(apvts, ParameterID::osc1Pos, osc1PosParam);
    castParameter(apvts, ParameterID::osc2Pos, osc2PosParam);
    castParameter(apvts, ParameterID::osc3Pos, osc3PosParam);
    castParameter(apvts, ParameterID::osc1Vol, osc1VolParam);
    castParameter(apvts, ParameterID::osc2Vol, osc2VolParam);
    castParameter(apvts, ParameterID::osc3Vol, osc3VolParam);
    castParameter(apvts, ParameterID::globalTrans, globalTransParam);
    castParameter(apvts, ParameterID::osc1Trans, osc1TransParam);
    castParameter(apvts, ParameterID::osc2Trans, osc2TransParam);
    castParameter(apvts, ParameterID::osc3Trans, osc3TransParam);
    apvts.state.addListener(this);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    synth.allocateResources(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    bool expected = true;
    if ( isNonRealtime() || parameterChanged.compare_exchange_strong(expected, false)) {
        update();
    }
    splitBufferEvents(buffer, midiMessages);

}

void AudioPluginAudioProcessor::update() {
    synth.osc1Index = osc1PosParam->get();
    synth.osc2Index = osc2PosParam->get();
    synth.osc3Index = osc3PosParam->get();
    synth.osc1Volume = juce::Decibels::decibelsToGain(osc1VolParam->get());
    synth.osc2Volume = juce::Decibels::decibelsToGain(osc2VolParam->get());
    synth.osc3Volume = juce::Decibels::decibelsToGain(osc3VolParam->get());
    synth.osc1Transpose = osc1TransParam->get();
    synth.globalTranspose = globalTransParam->get();
}

void AudioPluginAudioProcessor::splitBufferEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer) {
    int bufferOffset = 0;
    for (const auto message : midiBuffer) {
        int samplesB4Segment = message.samplePosition - bufferOffset;
        if (samplesB4Segment > 0) {
            render (buffer, samplesB4Segment, bufferOffset);
            bufferOffset += samplesB4Segment;
        }
        if (message.numBytes <= 3) {
           uint8_t data1 = (message.numBytes >= 2) ? message.data[1] : 0;
           uint8_t data2 = (message.numBytes == 3) ? message.data[2] : 0;
           handleMIDI(message.data[0], data1, data2);
        }
    }
    int samples4LastSegment = buffer.getNumSamples() - bufferOffset;
    if (samples4LastSegment > 0) {
        render(buffer, samples4LastSegment, bufferOffset);
    } 
    midiBuffer.clear();
}

void AudioPluginAudioProcessor::handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2) {
    char s[16];
    snprintf(s, 16, "%02hhX %02hhX %02hhX", data0, data1, data2);
    std::printf("%s\n", s);
    synth.midiMessage(data0, data1, data2);
}

void AudioPluginAudioProcessor::render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset) {
    float* outputBuffers[2] = {nullptr, nullptr};
    outputBuffers[0] = buffer.getWritePointer(0) + bufferOffset;
    if (getTotalNumOutputChannels() == 2) {
        outputBuffers[1] = buffer.getWritePointer(1) + bufferOffset;
    }
    synth.render(outputBuffers, sampleCount);
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::globalTrans,
                    "Global Transpose",
                    -12,
                    12,
                    0
                ));
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::osc1Pos,
                    "Oscillator 1 Position",
                    0, //min position
                    3, //max position
                    0
                ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                    ParameterID::osc1Vol,
                    "Oscillator 1 Volume",
                    -50.f,
                    6.f,
                    0.f
                ));
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::osc1Trans,
                    "Oscillator 1 Transpose",
                    -12,
                    12,
                    0
                ));
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::osc2Pos,
                    "Oscillator 2 Position",
                    0, //min position
                    3, //max position
                    0
                ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                    ParameterID::osc2Vol,
                    "Oscillator 2 Volume",
                    -50.f,
                    6.f,
                    0.f
                ));
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::osc2Trans,
                    "Oscillator 2 Transpose",
                    -12,
                    12,
                    0
                ));
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::osc3Pos,
                    "Oscillator 3 Position",
                    0, //min position
                    3, //max position
                    0
                ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                    ParameterID::osc3Vol,
                    "Oscillator 3 Volume",
                    -50.f,
                    6.f,
                    0.f
                ));
    layout.add(std::make_unique<juce::AudioParameterInt>(
                    ParameterID::osc3Trans,
                    "Oscillator 3 Transpose",
                    -12,
                    12,
                    0
                ));
    return layout;
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
   // return new AudioPluginAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
