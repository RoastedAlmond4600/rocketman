#pragma once

class RMHorizontalSlider : public juce::Component {
    public:
        RMHorizontalSlider();
        ~RMHorizontalSlider();
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RMHorizontalSlider)
};
