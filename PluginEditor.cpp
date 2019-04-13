/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerAudioProcessorEditor::FlangerAudioProcessorEditor(FlangerAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);

	auto& params = processor.getParameters();


	AudioParameterFloat* rate = (AudioParameterFloat*)params.getUnchecked(2);
	rateslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	rateslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	rateslid.setRange(rate->range.start, rate->range.end);
	rateslid.setValue(*rate);
	addAndMakeVisible(rateslid);
	rateslid.onValueChange = [this, rate] {*rate = rateslid.getValue();};
	rateslid.onDragStart = [rate] {rate->beginChangeGesture();};
	rateslid.onDragEnd = [rate] {rate->endChangeGesture();};
	rateslid.setLookAndFeel(&newlook);

	AudioParameterFloat* depth = (AudioParameterFloat*)params.getUnchecked(3);
	depthslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	depthslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	depthslid.setRange(depth->range.start, depth->range.end);
	depthslid.setValue(*depth);
	addAndMakeVisible(depthslid);
	depthslid.onValueChange = [this, depth] {*depth = depthslid.getValue();};
	depthslid.onDragStart = [depth] {depth->beginChangeGesture();};
	depthslid.onDragEnd = [depth] {depth->endChangeGesture();};
	depthslid.setLookAndFeel(&newlook);


	AudioParameterFloat* feed = (AudioParameterFloat*)params.getUnchecked(1);
	feedbackslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	feedbackslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	feedbackslid.setRange(feed->range.start, feed->range.end);
	feedbackslid.setValue(*feed);
	addAndMakeVisible(feedbackslid);
	feedbackslid.onValueChange = [this, feed] {*feed = feedbackslid.getValue();};
	feedbackslid.onDragStart = [feed] {feed->beginChangeGesture();};
	feedbackslid.onDragEnd = [feed] {feed->endChangeGesture();};
	feedbackslid.setLookAndFeel(&newlook);


	AudioParameterFloat* drywet = (AudioParameterFloat*)params.getUnchecked(0);
	drywetslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	drywetslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	drywetslid.setRange(drywet->range.start, drywet->range.end);
	drywetslid.setValue(*drywet);
	addAndMakeVisible(drywetslid);
	drywetslid.onValueChange = [this, drywet] {*drywet = drywetslid.getValue();};
	drywetslid.onDragStart = [drywet] {drywet->beginChangeGesture();};
	drywetslid.onDragEnd = [drywet] {drywet->endChangeGesture();};
	drywetslid.setLookAndFeel(&newlook);

	AudioParameterFloat* offset = (AudioParameterFloat*)params.getUnchecked(4);
	offslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	offslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	offslid.setRange(offset->range.start, offset->range.end);
	offslid.setValue(*offset);
	addAndMakeVisible(offslid);
	offslid.onValueChange = [this, offset] {*offset = offslid.getValue();};
	offslid.onDragStart = [offset] {offset->beginChangeGesture();};
	offslid.onDragEnd = [offset] {offset->endChangeGesture();};
	offslid.setLookAndFeel(&newlook);
}

FlangerAudioProcessorEditor::~FlangerAudioProcessorEditor()
{
}

//==============================================================================
void FlangerAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::darkmagenta);

	g.setColour(Colours::silver);
	g.setFont(15.0f);
	g.drawFittedText("Flanger 1.0 Albert Peyton 2019", getLocalBounds(), Justification::top, 1);
	Rectangle<int> box(getX(), getBottom() - 40, getWidth() / 5, 40);
	g.drawFittedText(TRANS("Rate(Hz)"), box, Justification::centred, 1);
	box.setX(box.getRight());
	g.drawFittedText(TRANS("Depth"), box, Justification::centred, 1);
	box.setX(box.getRight());
	g.drawFittedText(TRANS("Offset"), box, Justification::centred, 1);
	box.setX(box.getRight());
	g.drawFittedText(TRANS("Feedback"), box, Justification::centred, 1);
	box.setX(box.getRight());
	g.drawFittedText(TRANS("Dry/Wet"), box, Justification::centred, 1);
}

void FlangerAudioProcessorEditor::resized()
{
	auto box = getLocalBounds();
	box.setWidth(getWidth() / 5);
	box.setHeight(getHeight() - 40);
	rateslid.setBounds(box);
	box.setX(box.getRight());
	depthslid.setBounds(box);
	box.setX(box.getRight());
	offslid.setBounds(box);
	box.setX(box.getRight());
	feedbackslid.setBounds(box);
	box.setX(box.getRight());
	drywetslid.setBounds(box);
}
