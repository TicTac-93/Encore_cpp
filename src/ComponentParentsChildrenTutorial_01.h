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

 name:             ComponentParentsChildrenTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Explores relationships between components.

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
class SunComponent   : public juce::Component
{
public:
    SunComponent() {}

    void paint (juce::Graphics&) override {}

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SunComponent)
};

//==============================================================================
class FloorComponent   : public juce::Component
{
public:
    FloorComponent() {}

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::green);
        g.drawLine (0.0f, (float) getHeight() / 2.0f, (float) getWidth(), (float) getHeight() / 2.0f, 5.0f);
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloorComponent)
};

//==============================================================================
class WallComponent   : public juce::Component
{
public:
    WallComponent() {}

    void paint (juce::Graphics& g) override
    {
        g.fillCheckerBoard (getLocalBounds().toFloat(), 30, 10,
                            juce::Colours::sandybrown, juce::Colours::saddlebrown);
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WallComponent)
};

//==============================================================================
class RoofComponent   : public juce::Component
{
public:
    RoofComponent() {}

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::red);

        juce::Path roof;
        roof.addTriangle (0.0f, (float) getHeight(), (float) getWidth(), (float) getHeight(), (float) getWidth() / 2.0f, 0.0f);
        g.fillPath (roof);
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoofComponent)
};

//==============================================================================
class HouseComponent   : public juce::Component
{
public:
    HouseComponent()
    {
        addAndMakeVisible (wall);
        addAndMakeVisible (roof);
    }

    void resized() override
    {
        auto separation = juce::jlimit (2, 10, getHeight() / 20);                                                               // [1]

        roof.setBounds (0, 0, getWidth(), (int) (getHeight() * 0.2) - separation / 2);                                          // [2]
        wall.setBounds (0, (int) (getHeight() * 0.20) + separation / 2, getWidth(), (int) (getHeight() * 0.80) - separation);   // [3]
    }

private:
    WallComponent wall;
    RoofComponent roof;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HouseComponent)
};

//==============================================================================
class SceneComponent    : public juce::Component
{
public:
    SceneComponent()
    {
        addAndMakeVisible (floor);
        addAndMakeVisible (house);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::lightblue);
    }

    void resized() override
    {
        floor.setBounds (10, 297, 580, 5);
        house.setBounds (300, 70, 200, 220);
    }

private:
    FloorComponent floor;
    HouseComponent house;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneComponent)
};

//==============================================================================
class MainContentComponent   : public juce::Component
{
public:
    MainContentComponent()
    {
        addAndMakeVisible (scene);
        setSize (600, 400);
    }

    void paint (juce::Graphics&) override {}

    void resized() override
    {
        scene.setBounds (0, 0, getWidth(), getHeight());
    }

private:
    SceneComponent scene;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
