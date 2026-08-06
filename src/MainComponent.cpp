/*
  ==============================================================================

	Basic MainComponent class

  ==============================================================================
*/

#include "MainComponent.h"
#include <math.h>

//==============================================================================
MainComponent::MainComponent()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	setSize(600, 325);

	mainComponentFont = juce::Font(
		"Times New Roman",
		20.0f,
		juce::Font::italic | juce::Font::bold);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
	// Draw background
	g.fillAll(juce::Colours::lightblue);

	// Draw text
	g.setColour(juce::Colours::darkblue);
	g.setFont(mainComponentFont);
	g.drawText(
		currentSizeAsString,
		20, 40, 200, 40,
		juce::Justification::centred,
		true
	);

	// Draw dashed line
	g.setColour(juce::Colours::green);
	float dashes[2] = {10.0f, 5.0f};
	g.drawDashedLine(juce::Line(10.0f, 300.0f, 590.0f, 300.0f), dashes, 2, 5);

	// Draw "house"
	juce::Rectangle<float> house(300, 120, 200, 170);
	g.fillCheckerBoard(
		house,
		30, 10,
		juce::Colours::sandybrown,
		juce::Colours::saddlebrown
	);

	// Draw "sun"
	g.setColour(juce::Colours::yellow);
	DrawCircle(g, 500, 50, 30, 3);

	// Draw "roof"
	g.setColour(juce::Colours::red);
	juce::Path roof;
	roof.addTriangle(
		300, 110,
		500, 110,
		400, 70
	);
	g.fillPath(roof);
}

void MainComponent::resized()
{
	currentSizeAsString = juce::String(getWidth()) + " x " + juce::String(getHeight());
}

void MainComponent::DrawCircle(juce::Graphics& g, float centerX, float centerY,
	float radius, float thickness)
{
	g.drawEllipse(
		centerX - (radius * 0.5f),
		centerY - (radius * 0.5f),
		radius * 2,
		radius * 2,
		thickness
	);
}
