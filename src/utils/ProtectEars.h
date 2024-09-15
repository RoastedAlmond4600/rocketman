#pragma once
#include <JuceHeader.h>

template <typename T>
void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& dest) {
    dest = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(dest);
}

inline void protectEars(float* buffer, int sampleCount) {
    if (buffer == nullptr) { return; }
    bool firstWarning = true;
    for (int sample = 0; sample < sampleCount; ++sample) {
        float x = buffer[sample];
	bool silence = false;
	if (std::isnan(x)) {
	    printf("!!!WARRNING: Nan detected!. Silencing.\n");
	    silence = true;
	}
	else if (std::isinf(x)) {
	    printf("!!!WARNING: inf detected.\n");
	    silence = true;
	}
	else if (x < -2.0f || x > 2.0f) { // Screaming feedback
	    printf("Sample out of range!!!\n");
	    silence = true;
	}
	else if (x < -1.0f) {
	    if (firstWarning) {
		printf("!!!Warning. Sample is smaller than -1\n");
	        firstWarning = false;
	    }
	    buffer[sample] = -1.0f;
	}
	else if (x > 1.0f) {
	    if (firstWarning) {
		printf("!!!Warning. Sample is greater than 1.\n");
	        firstWarning = false;
	    }
	    buffer[sample] = 1.0f;
	}
	if (silence) {
	    memset(buffer, 0, sizeof(float) * sampleCount);
	    return;
	}
    }
}

