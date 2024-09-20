#pragma once

class OscillatorBaseComponent : public juce::Component {
    public:
        void paint(juce::Graphics& g) override {
            auto bounds = getLocalBounds().toFloat();
            g.setColour(juce::Colours::slategrey);
            g.fillRoundedRectangle(bounds, 5.f);
        }
        void resized() override {
        
        }
};
