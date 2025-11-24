#pragma once

#include <JuceHeader.h>

class NonsenseLookAndFeel : public juce::LookAndFeel_V4{
public:
	void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional,
		float rotaryStartAngle, float rotaryEndAngle, juce::Slider&);
	void fillCircleArcWithLines(float centerX, float centerY, float rInner, float rOuter, juce::Colour lineColor, juce::Graphics& g);
private:
};