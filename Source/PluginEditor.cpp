/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NonsenseAudioProcessorEditor::NonsenseAudioProcessorEditor(NonsenseAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    waveforms(p)
{
    waveformsViewport.setViewedComponent(&waveforms, false);

    waveformsViewport.setScrollBarThickness(30);

    waveformsViewport.getVerticalScrollBar().setColour(juce::ScrollBar::ColourIds::thumbColourId, NonsenseLookAndFeel::penColor);

    addAndMakeVisible(waveformsViewport);
    //waveformsViewport.addAndMakeVisible(waveforms);

    auto fira = juce::Typeface::createSystemTypefaceFor(BinaryData::FiraSansRegular_ttf, BinaryData::FiraSansRegular_ttfSize);



    juce::LookAndFeel::setDefaultLookAndFeel(&CustomLNF);

    CustomLNF.setDefaultSansSerifTypeface(fira);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
}

NonsenseAudioProcessorEditor::~NonsenseAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void NonsenseAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(NonsenseLookAndFeel::backgroundColor1);// getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

}

void NonsenseAudioProcessorEditor::resized()
{
    waveformsViewport.setBounds(0, 0, getWidth() * 0.7 + 30.0, getHeight());
    waveforms.setBounds(0, 0, getWidth() * 0.7, 1000);
}
