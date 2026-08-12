#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent final : public juce::Component
{
public:
	//==============================================================================
	MainComponent() {
		addAndMakeVisible(page_SelDir);

    	setSize(600, 400);
	}

	//==============================================================================
	void paint(juce::Graphics& g) override {
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(Colour(220, 220, 220));
	}

	void resized() override {
		page_SelDir.setBounds(getLocalBounds());
	}

private:
	//==============================================================================
	struct DirectorySelection : public Component {
		
		DirectorySelection() {
			lbl_currentDir.setColour(Label::backgroundColourId, Colours::white);
			lbl_currentDir.setColour(Label::textColourId, Colours::black);
			lbl_currentDir.setText("placeholder text for the label", NotificationType::dontSendNotification);

			btn_pickDir.setButtonText("Choose Directory...");

			addAndMakeVisible(lbl_currentDir);
			addAndMakeVisible(btn_pickDir);

			btn_pickDir.onClick = [this] { PickDirectory(); };
		}

		void paint(juce::Graphics& g) override {
			// g.fillAll(Colour(10, 10, 10));
		}

		void resized() override {
			using fb = FlexBox;

			fb box_main;
			box_main.flexDirection = fb::Direction::row;
			box_main.justifyContent = fb::JustifyContent::center;
			box_main.alignContent = fb::AlignContent::center;
			// box_main.alignItems = fb::AlignItems::center;
			box_main.items.add(FlexItem(lbl_currentDir).withFlex(1, 0).withHeight(20));
			box_main.items.add(FlexItem(btn_pickDir).withWidth(125).withHeight(25));
			
			fb box_outer;
			box_outer.flexDirection = fb::Direction::column;
			box_outer.justifyContent = fb::JustifyContent::center;
			box_outer.alignContent = fb::AlignContent::center;
			box_main.alignItems = fb::AlignItems::center;
			box_outer.items.add(FlexItem(box_main).withFlex(1, 1));
			
			box_main.performLayout(getLocalBounds());
			box_outer.performLayout(getLocalBounds().reduced(50));
		}

		Label lbl_currentDir = Label();
		TextButton btn_pickDir = TextButton();

		std::unique_ptr<FileChooser> dialog;

		/// @brief Spawns a 
		void PickDirectory() {
			printf("\nPickDirectory()");
			using fbc = FileBrowserComponent;

			dialog = std::make_unique<FileChooser>(
				"Select Directory to scan",
				File::getSpecialLocation(File::userDesktopDirectory),
				"");
			
			auto dialogFlags = fbc::openMode | fbc::canSelectDirectories;
			dialog->launchAsync(dialogFlags, [this](const FileChooser& chooser) {
				DirectoryHandler(chooser.getResult());
			});
		};

		void DirectoryHandler(File selection) {
			std::cout << "\n" << selection.getFullPathName().toStdString();
		}
	};
	
	//==============================================================================
	DirectorySelection page_SelDir = DirectorySelection();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
