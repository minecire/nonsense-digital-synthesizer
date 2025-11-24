
#include "Square.h"

void Square::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::red);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Wawawawawawa", getLocalBounds(), juce::Justification::centred, 1);
}

void Square::resized()
{
}