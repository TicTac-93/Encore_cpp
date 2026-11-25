#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/dc.h>
#include <wx/generic/statbmpg.h>
#include <wx/progdlg.h>

#include <format>
#include <vector>
#include "../internal/SequenceImporter.h"
#include "../internal/ImageImporter.h"
#include "../EncoreEnums.h"

// #include <chrono>

using namespace encore;

/// @brief A Dialog window to display a wxBitmap
class ThumbnailViewer : public wxDialog {
	public:
		/// @brief A Dialog window displaying the given bitmap, initialized to fit within the window.
		/// @param parent The parent window used for wxWidgets
		/// @param w_id ID to assign this window (not currently used)
		/// @param bitmap The wxBitmap to display
		/// @param title Text to display in the main window bar
		ThumbnailViewer(wxWindow *parent, int w_id, wxBitmap *bitmap, wxString title = "")
			: wxDialog(
				parent, w_id, title, wxDefaultPosition, wxDefaultSize,
				wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxMAXIMIZE_BOX)
		{
			aspect = ((float)bitmap->GetWidth() / (float)bitmap->GetHeight());

			// Set window sizing, accounting for padding and styling
			auto bitmap_size = bitmap->GetSize();
			auto dialog_size = wxGetDisplaySize();
			dialog_size *= 0.75f;
			if (dialog_size.GetWidth() > bitmap_size.GetWidth()) {
				dialog_size = bitmap_size;
			}
			// SetMaxSize(bitmap_size + window_padding);
			SetSize(dialog_size + window_padding);
			
			// Construction
			// SetBackgroundColour(wxColor("#3b3b3b"));
			bmp_display = new wxGenericStaticBitmap(this, -1, *bitmap);
			bmp_display->SetMaxSize(bitmap->GetSize());
			bmp_display->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFit);

			// Layout
			auto *layout = new wxBoxSizer(wxVERTICAL);
			layout->Add(bmp_display, 0, wxEXPAND | wxALL, PAD_MED);

			SetSizer(layout);
			Layout();
			// layout->Fit(this);

			// Connections
			Bind(wxEVT_SIZE, ThumbnailViewer::OnResize, this);
		}

		~ThumbnailViewer() {
			// No need for cleanup, all elements should be cleaned up by wxWidgets
		}

		void OnClose() {
			Destroy();
		}

	private:
		wxSize window_padding = wxSize(PAD_MED * 2, PAD_MED * 2 + wxSystemSettings::GetMetric(wxSYS_CAPTION_Y));
		float aspect = 0.f;

		wxGenericStaticBitmap *bmp_display;  // We will NOT own the bitmap being displayed
		                                     // Be careful not to delete it


		void OnResize(wxSizeEvent &event) {
			Layout();
			Refresh();
		}
};

/// @brief A wxGenericStaticBitmap extended with a mouse-click handler that will propogate up the heirarchy.
class ClickableStaticBitmap : public wxGenericStaticBitmap {
	public:
		/// @brief A wxGenericStaticBitmap that generates a wxEVT_BUTTON on left-click.
		/// @param parent The parent window used by wxWidgets
		/// @param w_id The ID for wxWidgets events
		/// @param bitmap The wxBitmap to display
		/// @param image_size The size to initialize the StaticBitmap with
		ClickableStaticBitmap(wxWindow *parent, int w_id, wxBitmap bitmap, wxSize image_size)
			: wxGenericStaticBitmap(parent, w_id, bitmap, wxDefaultPosition, image_size) 
		{
			// We only need to add a handler for mouse events, and then propogate them up
			Bind(wxEVT_LEFT_UP, ClickableStaticBitmap::OnMouseClick, this, -1);
		}

	private:
		void OnMouseClick(wxMouseEvent &event) {
			auto *btn_event = new wxCommandEvent(wxEVT_BUTTON, GetId());

			ProcessEvent(*btn_event);
		}
};

/// @brief A custom wxPanel-based widget for displaying a frame, its neighbors, and some stats.
class FrameWidget : public wxPanel {
	public:
		// FrameWidget() = default;

		/// @brief A wxPanel that displays thumbnails for a frame and its neighbors, some stats, and buttons for manually flagging.
		///        The buttons generate wxEVT_BUTTON events with the id ID_BTN_FLAG_ERR and ID_BTN_FLAG_OK, propogated up from here.
		///        The frame thumbnails are clickable, and expand into dialog windows for inspection.
		/// @param parent The parent used by wxWidgets
		/// @param w_id The ID to assign this widget
		/// @param prev_frame The bitmap to be displayed in the "previous" slot
		/// @param this_frame The bitmap to be displayed in the "current" slot
		/// @param next_frame The bitmap to be displayed in the "next" slot
		FrameWidget(wxWindow *parent, int w_id, wxBitmap *prev_frame, wxBitmap *this_frame, wxBitmap *next_frame)
			: wxPanel(parent, w_id)
		{
			// Construction
			p_status = new wxPanel(this, -1, wxDefaultPosition, wxSize(PAD_MED, -1));
			p_status->SetBackgroundColour(wxColor("#6ebe63"));
			lbl_frameNum = new wxStaticText(this, -1, "###");
			btn_markError = new wxButton(this, ID_BTN_FLAG_ERR, "Mark Error");
			btn_markError->SetHelpText("Mark this frame as an error, so it will be included in the frame list regardless of the current filter.");
			btn_markOK = new wxButton(this, ID_BTN_FLAG_OK, "Mark OK");
			btn_markError->SetHelpText("Mark this frame as OK, so it will be excluded from the frame list regardless of the current filter.");
			lbl_filename = new wxStaticText(this, -1, "###");
			lbl_statA = new wxStaticText(this, -1, "XXX", wxDefaultPosition, wxDefaultSize, wxTEXT_ALIGNMENT_RIGHT);
			lbl_statB = new wxStaticText(this, -1, "XXX");
			
			bmp_last = prev_frame;
			bmp_current = this_frame;
			bmp_next = next_frame;
			_frameAspect = (float)bmp_current->GetWidth() / (float)bmp_current->GetHeight();
			_imageSize = wxSize((int)(_frameAspect * 50), 50);

			thmb_last = new ClickableStaticBitmap(this, ID_IMG_LAST, *prev_frame, _imageSize);
			thmb_current = new ClickableStaticBitmap(this, ID_IMG_THIS, *this_frame, _imageSize);
			thmb_next = new ClickableStaticBitmap(this, ID_IMG_NEXT, *next_frame, _imageSize);
			thmb_last->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFill);
			thmb_current->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFill);
			thmb_next->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFill);
			SetBackgroundColour(wxColor("#eeeeee"));

			
			// Layout
			auto *layout = new wxBoxSizer(wxHORIZONTAL);
			layout->Add(thmb_last, 0, wxEXPAND);
			layout->Add(lbl_last, 0, wxALIGN_CENTER | wxALL, PAD_SML);
			layout->Add(thmb_current, 0, wxEXPAND);
			layout->Add(lbl_next, 0, wxALIGN_CENTER | wxALL, PAD_SML);
			layout->Add(thmb_next, 0, wxEXPAND);

			auto *vbox_1 = new wxBoxSizer(wxVERTICAL);
			vbox_1->Add(btn_markOK, 1, wxEXPAND);
			vbox_1->AddSpacer(PAD_SML);
			vbox_1->Add(btn_markError, 1, wxEXPAND);

			layout->AddSpacer(PAD_MED);
			layout->Add(vbox_1, 0, wxEXPAND);

			auto *vbox_2 = new wxBoxSizer(wxVERTICAL);
			vbox_2->Add(lbl_filename, 0, wxEXPAND);
			vbox_2->Add(lbl_statA, 0, wxEXPAND);
			vbox_2->Add(lbl_statB, 0, wxEXPAND);
			vbox_2->AddStretchSpacer();
			
			layout->AddStretchSpacer();
			layout->Add(vbox_2, 0, wxEXPAND);

			auto *hbox_1 = new wxBoxSizer(wxHORIZONTAL);
			hbox_1->Add(lbl_frameNum, 0, wxALIGN_TOP | wxTOP, PAD_MED);
			hbox_1->AddSpacer(PAD_SML);
			hbox_1->Add(p_status, 0, wxEXPAND);
			hbox_1->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);

			SetSizer(hbox_1);
			hbox_1->Fit(this);


			// Connections
			Bind(wxEVT_BUTTON, FrameWidget::OnMarkedError, this, ID_BTN_FLAG_ERR);
			Bind(wxEVT_BUTTON, FrameWidget::OnMarkedOK, this, ID_BTN_FLAG_OK);
			Bind(wxEVT_BUTTON, FrameWidget::OnClickedThumbnail, this, ID_IMG_LAST);
			Bind(wxEVT_BUTTON, FrameWidget::OnClickedThumbnail, this, ID_IMG_THIS);
			Bind(wxEVT_BUTTON, FrameWidget::OnClickedThumbnail, this, ID_IMG_NEXT);
		}

		/// @brief Sets the frame number to display on this widget, with padding.
		/// @param number The number to display
		/// @param padding The number of digits to pad the number to
		void SetFrameNum(int number, int padding=4) {
			_frameNum = number;
			std::string padded = std::to_string(number);
			while (padded.length() < padding) {
				padded = "0" + padded;
			}

			lbl_frameNum->SetLabel(padded);
		}

		/// @return The assigned frame number
		int GetFrameNum() {
			return _frameNum;
		}

		/// @param value The frame name to display, typically the full filename
		void SetName(std::string value) {
			lbl_filename->SetLabel(value);
		}

		/// @brief Sets the values for the stat display, formatted automatically.
		void SetStatLabels(float mean_a, float mean_b) {
			lbl_statA->SetLabel(std::format("Diff Prev: {:.4f}", mean_a));
			lbl_statB->SetLabel(std::format("Diff Next: {:.4f}", mean_b));
		}

		/// @param flagged Whether or not this frame was flagged by the program
		void SetFlagged(bool flagged) {
			_isFlagged = flagged;
			UpdateStatusBar();
		}

		/// @brief Adjust the size of the thumbnails being displayed.
		///        Called by the parent UI when the slider is adjusted.
		/// @param value A scale value from 0-100 inclusive
		void SetThumbSize(int value) {
			// Input value will be range 0-100
			// Remap it and apply to our base _imageSize
			float scale = (value * 0.05f) + 1;
			auto scaled_size = _imageSize * scale;
			
			thmb_last->SetMinSize(scaled_size);
			thmb_current->SetMinSize(scaled_size);
			thmb_next->SetMinSize(scaled_size);

			Fit();
			Refresh();
			// Update();
		}

		/// @return Whether or not the frame was flagged by the program
		bool IsFlagged() {
			return _isFlagged;
		}

		/// @return Whether the user marked this frame as an error
		bool IsMarkedError() {
			return _isMarkedError;
		}

		/// @return Whether the user marked this frame as OK
		bool IsMarkedOK() {
			return _isMarkedOK;
		}

	private:
		int _frameNum = -1;
		float _frameAspect = 1.f;
		wxSize _imageSize;
		float _diffLast = 0.f;
		float _diffNext = 0.f;
		bool _isFlagged = false; // Set by the flagging algorithm and threshold
		bool _isMarkedError = false; // Manually set by user, overrides flagging
		bool _isMarkedOK = false; // Manually set by user, overrides flagging

		wxPanel *p_status;
		ClickableStaticBitmap *thmb_last, *thmb_current, *thmb_next;
		wxStaticText *lbl_frameNum;
		wxBitmap *bmp_current, *bmp_last, *bmp_next;  // These should reference a central array of bitmaps, NOT new objs!
		wxButton *btn_markError;
		wxButton *btn_markOK;
		wxStaticText *lbl_filename;
		wxStaticText *lbl_statA;
		wxStaticText *lbl_statB;

		wxStaticText *lbl_last = new wxStaticText(this, -1, "<");
		wxStaticText *lbl_next = new wxStaticText(this, -1, ">");

		/// @brief Updates the color of the status bar based on the current flags
		void UpdateStatusBar() {

			if (_isMarkedError) {			
				p_status->SetBackgroundColour(wxColor("#ee6259"));
			}
			else if (_isMarkedOK) {
				p_status->SetBackgroundColour(wxColor("#6ebe63"));
			}
			else if (_isFlagged) {
				p_status->SetBackgroundColour(wxColor("#f7aa37"));
			}
			else {
				p_status->SetBackgroundColour(wxColor("#6ebe63"));
			}

			p_status->Refresh();
			Update();
		}

		/// @brief Handler for the "Mark Error" button
		void OnMarkedError(wxCommandEvent &event) {
			_isMarkedError = !_isMarkedError;
			_isMarkedOK = false;
			UpdateStatusBar();
			event.Skip();
		}
		
		/// @brief Handler for the "Mark OK" button
		void OnMarkedOK(wxCommandEvent &event) {
			_isMarkedOK = !_isMarkedOK;
			_isMarkedError = false;
			UpdateStatusBar();
			event.Skip();
		}

		/// @brief Handler for the clickable thumbnails, spawning a dialog based on which one was clicked.
		void OnClickedThumbnail(wxCommandEvent &event) {
			int id = event.GetId();
			wxString title = lbl_filename->GetLabelText();
			wxBitmap *bitmap;

			// We're handling multiple events here, so do some filtering
			switch (id) {
				case (ID_IMG_LAST):
					bitmap = bmp_last;
					title.append(" - PREV");
					break;

				case (ID_IMG_THIS):
					bitmap = bmp_current;
					break;

				case (ID_IMG_NEXT):
					bitmap = bmp_next;
					title.append(" - NEXT");
					break;

				default:
					event.Skip();
					return;
			}

			auto *img_dialog = new ThumbnailViewer(this, -1, bitmap, title);
			img_dialog->Show();
		}

};

class FrameAnalysisPanel : public wxPanel {
	public:
		FrameAnalysisPanel(wxWindow *parent)
			: wxPanel(parent)
		{
			// Construction
			lbl_frameCount = new wxStaticText(this, -1, "");
			chk_showOnlyFlagged = new wxCheckBox(this, ID_CHK_ONLYFLAGGED,
				wxT("Only show Flagged frames"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
			chk_showOnlyFlagged->SetValue(hideUnflagged);
			lbl_thumbSlider = new wxStaticText(this, -1, "Thumbnail Size");
			sld_thumbnailSize = new wxSlider(this, ID_SLD_THUMBSIZE, 2, 0, 5,
				wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS);
			lbl_thresh = new wxStaticText(this, -1, "Filter Threshold:");
			spn_thresh = new wxSpinCtrlDouble(this, ID_SPN_THRESH, wxEmptyString,
				wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT || wxTE_PROCESS_ENTER,
				0.0, 100.0, 5.0, 0.1);
			chk_autoThresh = new wxCheckBox(this, ID_CHK_AUTOTHRESH,
				wxT("Auto"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
			chk_autoThresh->SetValue(true);
			te_flaggedFrames = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize,
				wxTE_READONLY);

			vb_frameList = new wxBoxSizer(wxVERTICAL);
			p_frameList = new wxScrolled<wxPanel>(this);
			// p_frameList->SetBackgroundColour(wxColor("#aaaaaa"));
			p_frameList->SetSizer(vb_frameList);
			p_frameList->SetScrollRate(SCROLL_RATE, SCROLL_RATE);

			bmp_temp = new wxBitmap();
			bmp_temp->Create(960, 540, -1);


			// Layout
			auto *layout = new wxBoxSizer(wxVERTICAL);
			auto *layout_outer = new wxBoxSizer(wxHORIZONTAL);

			auto *hb_top = new wxBoxSizer(wxHORIZONTAL);
			hb_top->Add(chk_showOnlyFlagged, 0, wxALIGN_CENTER);
			hb_top->AddStretchSpacer();
			hb_top->Add(lbl_thumbSlider, 0, wxALIGN_CENTER);
			hb_top->Add(sld_thumbnailSize, 0);
			hb_top->AddStretchSpacer();
			hb_top->Add(lbl_thresh, 0, wxALIGN_CENTER);
			hb_top->Add(spn_thresh, 0, wxALIGN_CENTER);
			hb_top->AddSpacer(PAD_SML);
			hb_top->Add(chk_autoThresh, 0, wxALIGN_CENTER);

			layout->Add(lbl_frameCount, 0, wxEXPAND);
			layout->Add(hb_top, 0, wxEXPAND);
			layout->AddSpacer(PAD_MED);
			layout->Add(p_frameList, 1, wxEXPAND);
			layout->AddSpacer(PAD_MED);
			layout->Add(te_flaggedFrames, 0, wxEXPAND);

			layout_outer->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);
			SetSizer(layout_outer);
			layout_outer->Fit(this);


			// Connections
			Bind(wxEVT_SPINCTRLDOUBLE, FrameAnalysisPanel::OnThresholdChange, this, ID_SPN_THRESH);
			Bind(wxEVT_TEXT, FrameAnalysisPanel::OnThresholdChange, this, ID_SPN_THRESH);
			Bind(wxEVT_CHECKBOX, FrameAnalysisPanel::OnOnlyFlaggedChange, this, ID_CHK_ONLYFLAGGED);
			Bind(wxEVT_CHECKBOX, FrameAnalysisPanel::OnAutoThreshChange, this, ID_CHK_AUTOTHRESH);
			Bind(wxEVT_BUTTON, FrameAnalysisPanel::UpdateFlaggedFrameLabel, this, ID_BTN_FLAG_ERR);
			Bind(wxEVT_BUTTON, FrameAnalysisPanel::UpdateFlaggedFrameLabel, this, ID_BTN_FLAG_OK);
			Bind(wxEVT_SLIDER, FrameAnalysisPanel::OnThumbSizeChange, this, ID_SLD_THUMBSIZE);
		}

		~FrameAnalysisPanel() {
			delete bmp_temp;
			
			// Clear the contents of the frameData vector
			for (int i = 0; i < frameCount; i++) {
				delete frameData->at(i);
			}
			delete frameData;
			delete frameGroup;
		}

		/// @brief Assign a FrameGroup to be displayed by this.  Triggers file loading and analysis.
		/// @param frame_group Pointer to an initialized FrameGroup.  The frames within will be loaded and used to populate the UI. 
		/// @param downsampling The factor to downsample by during analysis.  1 = full, 2 = half, 4 = quarter etc.
		void SetFrameGroup(FrameGroup *frame_group, int downsampling) {
			frameGroup = frame_group;
			SetFrameCount(frameGroup->GetCount());
			framePadding = std::to_string(frameCount).length();

			// Spawn progress dialog
			wxProgressDialog prog_dialog("Loading Frame Sequence...", "", frameCount, this,
				wxPD_APP_MODAL);

			// Load images in the group
			LoadAndAnalyzeGroup(*frameGroup, *frameData, downsampling, &prog_dialog);

			p_frameList->Freeze();
			for (int i = 0; i < frameCount; i++) {
				prog_dialog.Update(i, std::format("Updating UI..."));

				auto tgtFramePath = frameGroup->GetFrameAt(i);
				auto tgtFrameData = frameData->at(i);
				auto newFrame = AddFrame(tgtFramePath->GetIndex());
				// Style frame widget
				newFrame->SetName(tgtFramePath->GetFilepath().filename().string());
				newFrame->SetStatLabels(tgtFrameData->a_mean, tgtFrameData->b_mean);
			}

			// Update thumbnail sizes based on slider
			UpdateThumbSize();

			// Flag images using analysis data and current threshold
			UpdateAutoThresh();
			UpdateFrameFlags();

			prog_dialog.Close();
			prog_dialog.Destroy();

			Layout();
			p_frameList->Thaw();
		}

	private:
		wxStaticText *lbl_frameCount;
		wxCheckBox *chk_showOnlyFlagged;
		wxStaticText *lbl_thumbSlider;
		wxSlider *sld_thumbnailSize;
		wxStaticText *lbl_thresh;
		wxCheckBox *chk_autoThresh;
		wxSpinCtrlDouble *spn_thresh;
		wxScrolled<wxPanel> *p_frameList;
		wxBoxSizer *vb_frameList;
		wxTextCtrl *te_flaggedFrames;
		wxBitmap *bmp_temp;
		
		int frameCount = 0;
		int framePadding = 4;
		int flaggedCount = 0;
		float threshold = 0.05f;
		float auto_thresh = 0.0f;
		bool hideUnflagged = true;

		int SCROLL_RATE = 10;

		std::vector<FrameWidget*> *frameWidgets = new std::vector<FrameWidget*>;
		std::vector<ImageData*> *frameData = new std::vector<ImageData*>;
		FrameGroup *frameGroup = nullptr;

		/// @brief Updates the frame / flagged count label based on current conditions
		void UpdateCountLabel() {
			lbl_frameCount->SetLabel(std::format("{} Frames ({} Flagged)", frameCount, flaggedCount));
		}

		/// @brief Sets the frameCount and re-initializes several aspects of the UI and memory
		void SetFrameCount(int count) {
			frameCount = count;

			// Widgets
			p_frameList->DestroyChildren();
			frameWidgets->clear();
			frameWidgets->assign(count, nullptr);

			// Image Data
			for (int i = 0; i < frameData->size(); i++) {
				delete frameData->at(i);
			}
			frameData->clear();
			frameData->assign(count, nullptr);
		}

		/// @brief Constructs a new FrameWidget and adds it to the UI
		FrameWidget* AddFrame(int index) {
			wxBitmap *bmp_last, *bmp_next;
			if (index > 0) {
				bmp_last = frameData->at(index - 1)->Bitmap();
			} else {
				bmp_last = bmp_temp;
			}

			if (index + 1 < frameGroup->GetCount()) {
				bmp_next = frameData->at(index + 1)->Bitmap();
			} else {
				bmp_next = bmp_temp;
			}

			auto *newFrame = new FrameWidget(p_frameList, -1, bmp_last, frameData->at(index)->Bitmap(), bmp_next);
			int frame_num = frameGroup->GetFrameAt(index)->GetIndex(); // Gets the actual frame number, in case the sequence is not 0-indexed
			newFrame->SetFrameNum(frame_num, framePadding);
			frameWidgets->at(index) = newFrame;
			vb_frameList->Add(newFrame, 0, wxEXPAND);

			return newFrame;
		}

		/// @brief Recalculates frame flagging, typically after the threshold has been changed.
		void UpdateFrameFlags() {
			int flag_count = 0;

			for (int i = 0; i < frameCount; i++) {
				auto widget = frameWidgets->at(i);
				auto frame = frameData->at(i);
				ImageData* prev = nullptr;
				ImageData* next = nullptr;
				float prev_a = 0.f;
				float next_b = 0.f;
				bool flag = false;

				// Prev frame data
				if (i > 0) {
					prev = frameData->at(i-1);
					prev_a = prev->a_mean;
				}

				// Next frame data
				if (i+1 < frameCount) {
					next = frameData->at(i+1);
					next_b = next->b_mean;
				}

				// Test vs prev frame
				if (prev != nullptr
				&&  prev_a > 0
				&&  frame->a_mean > prev_a
				&&  frame->a_mean - prev_a > threshold * prev_a) {
					flag = true;
				}

				// Test vs next frame
				if (!flag
				&&  next != nullptr
				&&  next_b > 0
				&&  frame->b_mean > next_b
				&&  frame->b_mean - next_b > threshold * next_b) {
					flag = true;
				}

				widget->SetFlagged(flag);
				if (flag) flag_count ++;
			}
			
			flaggedCount = flag_count;
			UpdateCountLabel();
			UpdateFlaggedFrameLabel();
			HideUnflaggedFrames(hideUnflagged);
		}

		/// @brief Show or Hide unflagged frames in the main UI.
		/// @param state Whether UNFLAGGED frames should be hidden or not.
		void HideUnflaggedFrames(bool state) {
			p_frameList->Freeze();

			if (state) {
				for (int i = 0; i < frameCount; i++) {
					auto frame = frameWidgets->at(i);
					
					if (frame->IsFlagged()
					||  frame->IsMarkedError() ) {
						frame->Show();
					} else {
						frame->Hide();
					}
				}
			} else {
				for (int i = 0; i < frameCount; i++) {
					frameWidgets->at(i)->Show();
				}
			}

			p_frameList->Layout();
			p_frameList->Thaw();
			Layout();
		}

		/// @brief Re-calculate the auto-threshold value for flagging frames.
		///        This should only need to be done once, after loading a frame group.
		void UpdateAutoThresh() {
			// Update the threshold based on the standard deviation of diff values

			// TODO: Detect outlier values and attempt to cull them from the dataset
			// The idea is to guard against one or two outsized glitches "hiding" many
			// normal ones in the auto-filter, or pegging the threshold at 100%


			// Get total mean
			float total_mean_diff = 0.f;
			float mean_diffs[frameCount-1];
			float mean_of_means = 0.f; // Used for setting the auto threshold
			for (int i = 0; i < frameCount - 1; i++) {
				mean_diffs[i] = fabs(frameData->at(i)->b_mean - frameData->at(i+1)->b_mean);
				total_mean_diff += mean_diffs[i];

				mean_of_means += frameData->at(i)->b_mean;
			}
			total_mean_diff /= (frameCount - 1);
			mean_of_means /= (frameCount - 1);

			// Update mean_diffs to be difference from total_mean_diff
			// Simultaneously square those values and sum them
			float stdev = 0.f;
			for (int i = 0; i < frameCount - 1; i++) {
				mean_diffs[i] -= total_mean_diff;
				stdev += (mean_diffs[i] * mean_diffs[i]);
			}

			// Divide by our sample size (frameCount-1) and get sqrt for standard deviation
			stdev /= (frameCount - 1);
			stdev = sqrtf(stdev);

			// Update threshold value and GUI
			// Half of one standard deviation works fairly well
			auto_thresh = stdev / mean_of_means / 2;
			OnAutoThreshChange(false);
		}

		void UpdateFlaggedFrameLabel() {
			std::string output = "";
			int count = 0;
			for (int i = 0; i < frameCount; i++) {
				auto widget = frameWidgets->at(i);

				if (!widget->IsMarkedOK()
				&& ( widget->IsFlagged() || widget->IsMarkedError() )) {
					output.append(std::format("{}, ", widget->GetFrameNum()));
					count ++;
				}
			}

			// Remove trailing ", "
			output.resize(output.size() - 2);
			te_flaggedFrames->SetLabel(output);
			
			flaggedCount = count;
			UpdateCountLabel();
		}

		/// @brief Adjust the size of all FrameWidget thumbnails based on the slider
		void UpdateThumbSize() {
			int slider_val = sld_thumbnailSize->GetValue() * 20;

			for (int i = 0; i < frameCount; i++) {
				frameWidgets->at(i)->SetThumbSize(slider_val);
			}
			p_frameList->FitInside();
			p_frameList->Refresh();
		}

		// Event handlers

		/// @brief Handler for when the threshold spinner value changes.
		///
		///        Calls UpdateFrameFlags()
		void OnThresholdChange(wxCommandEvent &event) {
			threshold = std::stof(spn_thresh->GetTextValue().ToStdString()) / 100;
			UpdateFrameFlags();
		}

		/// @brief Handler for the "Show Only Flagged Frames" button
		void OnOnlyFlaggedChange(wxCommandEvent &event) {
			hideUnflagged = chk_showOnlyFlagged->IsChecked();

			HideUnflaggedFrames(hideUnflagged);
		}

		/// @brief Handler for the "Use Auto Threshold" button.
		///
		///        Sets the internal threshold value accordingly and updates flags.
		void OnAutoThreshChange(bool update_flags=true) {
			if (chk_autoThresh->GetValue()) {
				threshold = auto_thresh;
				spn_thresh->SetValue(auto_thresh * 100); // Spinner uses scaled values for readability
				spn_thresh->Disable();
				
				if (update_flags) UpdateFrameFlags();
			} else {
				spn_thresh->Enable();
			}

		}

		/// @brief Handler for the "Thumbnail Size" slider.
		void OnThumbSizeChange(wxCommandEvent &event) {
			UpdateThumbSize();
		}

		// Wrapper for Binding to ui event
		void OnAutoThreshChange(wxCommandEvent &event) {
			OnAutoThreshChange();
		}

		// Wrapper for Binding to ui event
		void UpdateFlaggedFrameLabel(wxCommandEvent &event) {
			UpdateFlaggedFrameLabel();
		}
		
};