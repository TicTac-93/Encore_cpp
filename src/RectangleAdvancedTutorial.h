/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             RectangleAdvancedTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Displays rectangles.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics
 exporters:        xcode_mac, vs2019, linux_make, xcode_iphone, androidstudio

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public juce::Component
{
public:
    MainContentComponent()
    {

		// HEADER
        header.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
        header.setButtonText ("Header");
        addAndMakeVisible (header);

		// FOOTER
        footer.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
        footer.setButtonText ("Footer");
        addAndMakeVisible (footer);

		// SIDEBAR
        sidebar.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
        addAndMakeVisible (sidebar);

		sideItemA.setColour (juce::TextButton::buttonColourId, juce::Colours::maroon);
		sideItemB.setColour (juce::TextButton::buttonColourId, juce::Colours::maroon);
		sideItemC.setColour (juce::TextButton::buttonColourId, juce::Colours::maroon);
		sideItemA.setButtonText ("Item A");
		sideItemB.setButtonText ("Item B");
		sideItemC.setButtonText ("Item C");
		addAndMakeVisible (sideItemA);
		addAndMakeVisible (sideItemB);
		addAndMakeVisible (sideItemC);

		// MAIN CONTENT
        limeContent.setColour (juce::TextButton::buttonColourId, juce::Colours::lime);
        addAndMakeVisible (limeContent);

        grapefruitContent.setColour (juce::TextButton::buttonColourId, juce::Colours::yellowgreen);
        addAndMakeVisible (grapefruitContent);

        lemonContent.setColour (juce::TextButton::buttonColourId, juce::Colours::yellow);
        addAndMakeVisible (lemonContent);

        orangeContent.setColour (juce::TextButton::buttonColourId, juce::Colours::orange);
        addAndMakeVisible (orangeContent);

		redContent.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
		addAndMakeVisible (redContent);

		blueContent.setColour (juce::TextButton::buttonColourId, juce::Colours::blue);
		addAndMakeVisible (blueContent);

		greenContent.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
		addAndMakeVisible (greenContent);

        setSize (400, 400);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::darkgrey);
    }

    void resized() override
    {
        auto area = getLocalBounds();

        int headerFooterHeight = 36;
        header.setBounds (area.removeFromTop    (headerFooterHeight));
        footer.setBounds (area.removeFromBottom (headerFooterHeight));

        sidebar.setBounds (area.removeFromRight (juce::jlimit(100, 200, area.getWidth() / 4))); // [2]
		auto sidebarArea = sidebar.getBounds();
		int sideItemHeight = 40;
		int sideItemMargin = 5;
		sideItemA.setBounds (sidebarArea.removeFromTop (sideItemHeight).reduced(sideItemMargin));
		sideItemB.setBounds (sidebarArea.removeFromTop (sideItemHeight).reduced(sideItemMargin));
		sideItemC.setBounds (sidebarArea.removeFromTop (sideItemHeight).reduced(sideItemMargin));


        int contentItemHeight = 24;
        orangeContent.setBounds     (area.removeFromTop (contentItemHeight));
        limeContent.setBounds       (area.removeFromTop (contentItemHeight)); // [1]
        grapefruitContent.setBounds (area.removeFromTop (contentItemHeight));
        lemonContent.setBounds      (area.removeFromTop (contentItemHeight));

		auto areaChunk = area.getWidth() / 3;
		redContent.setBounds        (area.removeFromLeft (areaChunk));
		greenContent.setBounds      (area.removeFromLeft (areaChunk));
		blueContent.setBounds       (area.removeFromLeft (areaChunk));
    }

private:
    juce::TextButton header;

    juce::TextButton sidebar;
	juce::TextButton sideItemA;
	juce::TextButton sideItemB;
	juce::TextButton sideItemC;

    juce::TextButton limeContent;
    juce::TextButton grapefruitContent;
    juce::TextButton lemonContent;
    juce::TextButton orangeContent;
    juce::TextButton footer;

	juce::TextButton redContent;
	juce::TextButton blueContent;
	juce::TextButton greenContent;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
