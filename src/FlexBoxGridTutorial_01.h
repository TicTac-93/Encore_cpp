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

 name:			   FlexBoxGridTutorial
 version:		   1.0.0
 vendor:		   JUCE
 website:		   http://juce.com
 description:	   Responsive layouts using FlexBox and Grid.

 dependencies:	   juce_core, juce_data_structures, juce_events, juce_graphics,
				   juce_gui_basics
 exporters: 	   xcode_mac, vs2019, linux_make, xcode_iphone, androidstudio

 type:			   Component
 mainClass: 	   MainContentComponent

 useLocalCopy:	   1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainContentComponent	 : public juce::Component
{
public:
	//==============================================================================
	MainContentComponent()
		: rightPanel (juce::Colours::lightgrey),
		  leftPanel  (juce::Colours::lightblue)
	{
		addAndMakeVisible (rightPanel);
		addAndMakeVisible (leftPanel);
		addAndMakeVisible (mainPanel);

		setSize (600, 400);
	}

	void paint (juce::Graphics& g) override
	{
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	}

	void resized() override
	{
		// auto bounds = getLocalBounds();

		// auto panelWidth = bounds.getWidth() / 4;

		// leftPanel.setBounds  (bounds.removeFromLeft  (panelWidth));
		// rightPanel.setBounds (bounds.removeFromRight (panelWidth));

		// mainPanel.setBounds (bounds);

		// juce::FlexBox fb;
		// juce::FlexItem left ((float) getWidth() / 4.0f, (float) getHeight(), leftPanel);
		// juce::FlexItem right ((float) getWidth() / 4.0f, (float) getHeight(), rightPanel);
		// juce::FlexItem main ((float) getWidth() / 2.0f, (float) getHeight(), mainPanel);

		// fb.items.addArray( { left, main, right });
		// fb.performLayout(getLocalBounds());

		juce::Grid grid;
		using Track = juce::Grid::TrackInfo;
		using Fr = juce::Grid::Fr;
		using gi = juce::GridItem;
		grid.templateRows = { Track(Fr(1)) };
		grid.templateColumns = { Track(Fr(1)), Track(Fr(2)), Track(Fr(1)) };
		grid.items = { gi(leftPanel), gi(mainPanel), gi(rightPanel) };
		grid.performLayout(getLocalBounds());
	}

private:
	//==============================================================================
	struct RightSidePanel	 : public Component
	{
		RightSidePanel (juce::Colour c) : backgroundColour (c)
		{
			for (int i = 0; i < 9; ++i)
				addAndMakeVisible (buttons.add (new juce::TextButton (juce::String (i))));
		}

		void paint (juce::Graphics& g) override
		{
			g.fillAll (backgroundColour);
		}

		void resized() override
		{
			juce::FlexBox fb;
			fb.flexWrap = juce::FlexBox::Wrap::wrap;
			fb.justifyContent = juce::FlexBox::JustifyContent::center;
			fb.alignContent = juce::FlexBox::AlignContent::center;

			for (juce::TextButton* b : buttons) {
				fb.items.add(juce::FlexItem(*b).withMinWidth(50.0f).withMinHeight(50.0f));
			}
			
			fb.performLayout(getLocalBounds());

			// auto bounds = getLocalBounds();
			// auto buttonSize = bounds.getWidth() / 3;

			// for (int i = 0; i < buttons.size(); ++i)
			// {
			//	   buttons[i]->setBounds (buttonSize * (i % 3),
			//							  buttonSize * (i / 3) + bounds.getHeight() / 3,
			//							  buttonSize,
			//							  buttonSize);
			// }
		}

		juce::Colour backgroundColour;
		juce::OwnedArray<juce::TextButton> buttons;
	};

	struct LeftSidePanel	: public Component
	{
		LeftSidePanel (juce::Colour c) : backgroundColour (c)
		{
			for (int i = 0; i < 6; ++i)
			{
				auto* slider = new juce::Slider();
				slider->setSliderStyle (juce::Slider::SliderStyle::Rotary);
				slider->setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);

				addAndMakeVisible (knobs.add (slider));
			}
		}

		void paint (juce::Graphics& g) override
		{
			g.fillAll (backgroundColour);
		}

		void resized() override
		{
			juce::FlexBox knobBox;
			knobBox.flexWrap = juce::FlexBox::Wrap::wrap;
			knobBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

			for(juce::Slider* k : knobs) {
				knobBox.items.add(juce::FlexItem(*k).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
			}

			juce::FlexBox fb;
			fb.flexDirection = juce::FlexBox::Direction::column;
			fb.items.add(juce::FlexItem(knobBox).withFlex(2.5));

			fb.performLayout(getLocalBounds());

			// auto bounds = getLocalBounds();
			// auto knobSize = bounds.getWidth() / 3;

			// for (int i = 0; i < knobs.size(); ++i)
			// {
			// 	knobs[i]->setBounds (knobSize * (i % 3),
			// 						 bounds.getHeight() / 2 * (i / 3),
			// 						 knobSize,
			// 						 knobSize);
			// }
		}

		juce::Colour backgroundColour;
		juce::OwnedArray<juce::Slider> knobs;
	};

	struct MainPanel	: public Component
	{
		MainPanel()
		{
			for (int i = 0; i < 5; ++i)
			{
				addAndMakeVisible (sliders.add (new juce::Slider()));
				sliders.getLast()->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
			}
		}

		void paint (juce::Graphics& g) override
		{
			g.fillAll (juce::Colours::hotpink);
		}

		void resized() override
		{
			// auto bounds = getLocalBounds();

			// auto sliderHeight = bounds.getHeight() / sliders.size();

			// for (auto* slider : sliders)
			// {
			// 	slider->setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
			// 	slider->setBounds (bounds.removeFromTop (sliderHeight));
			// }

			auto isPortrait = ( getLocalBounds().getHeight() > getLocalBounds().getWidth() );
			juce::FlexBox fb;
			fb.flexDirection = 
				isPortrait
				? juce::FlexBox::Direction::column
				: juce::FlexBox::Direction::row;

			for(auto* s : sliders) {
				s->setSliderStyle(
					isPortrait
					? juce::Slider::SliderStyle::LinearHorizontal
					: juce::Slider::SliderStyle::LinearVertical);

				fb.items.add(juce::FlexItem(*s).withFlex(0, 1,
					isPortrait
					? (float) getHeight() / 5.0f
					: (float) getWidth() / 5.0f));
			}

			fb.performLayout(getLocalBounds());
		}

		juce::OwnedArray<juce::Slider> sliders;
	};

	//==============================================================================
	RightSidePanel rightPanel;
	LeftSidePanel leftPanel;
	MainPanel mainPanel;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
