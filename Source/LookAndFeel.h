#pragma once

#include <JuceHeader.h>

class NonsenseLookAndFeel : public juce::LookAndFeel_V4 {
public:
	NonsenseLookAndFeel();


	void drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height,
		bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, [[maybe_unused]] bool isMouseDown);

	void drawComboBox(juce::Graphics& g, int width, int height, bool,
		int a, int b, int c, int d, juce::ComboBox& box);
	void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
		const bool isSeparator, const bool isActive,
		const bool isHighlighted, const bool isTicked,
		const bool hasSubMenu, const juce::String& text,
		const juce::String& shortcutKeyText,
		const juce::Drawable* icon, const juce::Colour* const textColourToUse);


	void drawButtonBackground(juce::Graphics& g,
		juce::Button& button,
		const juce::Colour& backgroundColour,
		bool shouldDrawButtonAsHighlighted,
		bool shouldDrawButtonAsDown);

	void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown);

	void drawTickBox(juce::Graphics& g, juce::Component& component,
		float x, float y, float w, float h,
		const bool ticked,
		const bool isEnabled,
		const bool shouldDrawButtonAsHighlighted,
		const bool shouldDrawButtonAsDown) override;

	void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional,
		float rotaryStartAngle, float rotaryEndAngle, juce::Slider&);
	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider);

	const static juce::Colour backgroundColor1;
	const static juce::Colour backgroundColor2;
	const static juce::Colour backgroundColor3;

	const static juce::Colour penColor;
	const static juce::Colour NonsenseLookAndFeel::fancyPenColors[10];

private:
	void fillRectWithLines(juce::Graphics& g, int x, int y, int w, int h, float spacing);
	void fillCircleArcWithLines(float centerX, float centerY, float rInner, float rOuter, juce::Colour lineColor, float lineSpacing, float linesAngle, juce::Graphics& g);
	std::vector<float> findCircleLineIntersections(float lineX1, float lineY1, float lineX2, float lineY2, float circleCenterX, float circleCenterY, float circleInnerR, float circleOuterR);
};