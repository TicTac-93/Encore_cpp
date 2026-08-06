/*
  ==============================================================================

	Basic MainComponent class

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/*
*/
class MainComponent  : public juce::Component
{
public:
	MainComponent();
	~MainComponent() override;

	void paint (juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

	juce::String currentSizeAsString;
	juce::Font mainComponentFont;

	void DrawCircle(juce::Graphics& g, float centerX, float centerY,
		float radius, float thickness);
};
