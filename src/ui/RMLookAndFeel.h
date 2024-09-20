#pragma once

class RMLookAndFeel : public juce::LookAndFeel_V4 {
    public:
        RMLookAndFeel() {
            setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(juce::Colours::white));
        }
    private:
        void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& slider) override {
            
        }
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RMLookAndFeel)
};
