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

#include <chrono>

using namespace encore;

class FrameWidget : public wxPanel {
	public:
		FrameWidget() = default;

		FrameWidget(wxWindow *parent, int w_id, wxBitmap *prev_frame, wxBitmap *this_frame, wxBitmap *next_frame)
			: wxPanel(parent, w_id)
		{
			// Initialize internal vars to defaults
			_frameNum = -1;
			_diffLast = 0.0f;
			_diffNext = 0.0f;
			_isFlagged = false;


			// Construction
			p_status = new wxPanel(this, -1, wxDefaultPosition, wxSize(PAD_MED, -1));
			p_status->SetBackgroundColour(wxColor("#6ebe63"));
			lbl_frameNum = new wxStaticText(this, -1, "###");
			bmp_last = prev_frame;
			bmp_current = this_frame;
			bmp_next = next_frame;
			btn_flagFrame = new wxToggleButton(this, ID_BTN_FLAG, "Flag Error");
			lbl_elapsed = new wxStaticText(this, -1, "###");

			auto imageSize = wxSize(100, 56);
			p_last = new wxGenericStaticBitmap(this, ID_IMG_LAST, *prev_frame, wxDefaultPosition, imageSize);
			p_current = new wxGenericStaticBitmap(this, ID_IMG_THIS, *this_frame, wxDefaultPosition, imageSize);
			p_next = new wxGenericStaticBitmap(this, ID_IMG_NEXT, *next_frame, wxDefaultPosition, imageSize);
			p_last->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFill);
			p_current->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFill);
			p_next->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFill);
			SetBackgroundColour(wxColor("#eeeeee"));
			
			// Layout
			auto *layout = new wxBoxSizer(wxHORIZONTAL);
			layout->Add(p_last, 0, wxEXPAND);
			layout->Add(lbl_last, 0, wxALIGN_CENTER | wxALL, PAD_SML);
			layout->Add(p_current, 0, wxEXPAND);
			layout->Add(lbl_next, 0, wxALIGN_CENTER | wxALL, PAD_SML);
			layout->Add(p_next, 0, wxEXPAND);
			layout->AddSpacer(PAD_MED);
			layout->Add(btn_flagFrame, 0, wxALIGN_CENTER);
			layout->AddStretchSpacer();
			layout->Add(lbl_elapsed, 0, wxEXPAND);

			auto *hbox_1 = new wxBoxSizer(wxHORIZONTAL);
			hbox_1->Add(lbl_frameNum, 0, wxALIGN_TOP | wxTOP, PAD_MED);
			hbox_1->AddSpacer(PAD_SML);
			hbox_1->Add(p_status, 0, wxEXPAND);
			hbox_1->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);

			SetSizer(hbox_1);
			hbox_1->Fit(this);


			// Connections
		}

		void SetFrameNum(int number, int padding=4) {
			_frameNum = number;
			std::string padded = std::to_string(number);
			while (padded.length() < padding) {
				padded = "0" + padded;
			}

			lbl_frameNum->SetLabel(padded);
		}

		void SetName(std::string value) {
			lbl_elapsed->SetLabel(value);
		}

		void SetFlagged(bool flagged) {
			_isFlagged = flagged;

			if (flagged) {
				p_status->SetBackgroundColour(wxColor("#f7aa37"));
			}
			else {
				p_status->SetBackgroundColour(wxColor("#6ebe63"));
			}

			p_status->Refresh();
			Update();
		}

		bool IsFlagged() {
			return _isFlagged;
		}

	private:
		int _frameNum;
		float _diffLast, _diffNext;
		bool _isFlagged;

		wxPanel *p_status;
		wxGenericStaticBitmap *p_last, *p_current, *p_next;
		wxStaticText *lbl_frameNum;
		wxBitmap *bmp_current, *bmp_last, *bmp_next;  // These should reference a central array of bitmaps, NOT new objs!
		wxToggleButton *btn_flagFrame;
		wxStaticText *lbl_elapsed;

		wxStaticText *lbl_last = new wxStaticText(this, -1, "<");
		wxStaticText *lbl_next = new wxStaticText(this, -1, ">");
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
			lbl_thresh = new wxStaticText(this, -1, "Detection Threshold:");
			spn_thresh = new wxSpinCtrlDouble(this, ID_SPN_THRESH, wxEmptyString,
				wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT || wxTE_PROCESS_ENTER,
				0.0, 100.0, 5.0, 0.1);
			chk_autoThresh = new wxCheckBox(this, ID_CHK_AUTOTHRESH,
				wxT("Auto"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);

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
			hb_top->Add(lbl_thresh, 0, wxALIGN_CENTER);
			hb_top->Add(spn_thresh, 0);
			hb_top->Add(chk_autoThresh, 0, wxALIGN_CENTER);

			layout->Add(lbl_frameCount, 0, wxEXPAND);
			layout->Add(hb_top, 0, wxEXPAND);
			layout->AddSpacer(PAD_MED);
			layout->Add(p_frameList, 1, wxEXPAND);

			layout_outer->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);
			SetSizer(layout_outer);
			layout_outer->Fit(this);


			// Connections
			Bind(wxEVT_SPINCTRLDOUBLE, &FrameAnalysisPanel::OnThresholdChange, this, ID_SPN_THRESH);
			Bind(wxEVT_CHECKBOX, &FrameAnalysisPanel::OnOnlyFlaggedChange, this, ID_CHK_ONLYFLAGGED);
		}

		void SetFrameGroup(FrameGroup *value) {
			frameGroup = value;
			SetFrameCount(frameGroup->GetCount());
			framePadding = std::to_string(frameCount).length();

			// Spawn progress dialog
			wxProgressDialog prog_dialog("Loading Frame Sequence...", "", frameCount, this,
				wxPD_APP_MODAL);

			// Load images in the group
			LoadAndAnalyzeGroup(*frameGroup, *frameData, 4, &prog_dialog);


			p_frameList->Freeze();
			for (int i = 0; i < frameCount; i++) {
				prog_dialog.Update(i, std::format("Updating UI..."));

				auto tgtFramePath = frameGroup->GetFrameAt(i);
				auto newFrame = AddFrame(tgtFramePath->GetIndex());
				// Style frame widget
				newFrame->SetName(tgtFramePath->GetFilepath().stem().string());
			}

			// Flag images using analysis data and current threshold
			UpdateFrameFlags();

			Layout();
			p_frameList->Thaw();
		}

	private:
		wxStaticText *lbl_frameCount;
		wxCheckBox *chk_showOnlyFlagged;
		wxStaticText *lbl_thresh;
		wxCheckBox *chk_autoThresh;
		wxSpinCtrlDouble *spn_thresh;
		wxScrolled<wxPanel> *p_frameList;
		wxBoxSizer *vb_frameList;
		wxBitmap *bmp_temp;
		
		int frameCount = 0;
		int framePadding = 4;
		int flaggedCount = 0;
		float threshold = 0.05f;
		std::chrono::steady_clock *threshTimer = new std::chrono::steady_clock();
		std::chrono::time_point<std::chrono::steady_clock> threshLastUpdated = threshTimer->now();
		bool hideUnflagged = true;

		int SCROLL_RATE = 10;

		std::vector<FrameWidget*> *frameWidgets = new std::vector<FrameWidget*>;
		std::vector<ImageData*> *frameData = new std::vector<ImageData*>;
		FrameGroup *frameGroup = nullptr;

		void UpdateCountLabel() {
			lbl_frameCount->SetLabel(std::format("{} Frames ({} Flagged)", frameCount, flaggedCount));
		}

		void SetFrameCount(int count) {
			frameCount = count;
			UpdateCountLabel();

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
			newFrame->SetFrameNum(index, framePadding);
			frameWidgets->at(index) = newFrame;
			vb_frameList->Add(newFrame, 0, wxEXPAND);

			return newFrame;
		}

		void UpdateFrameFlags() {
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
			}
			
			HideUnflaggedFrames(hideUnflagged);
		}

		void HideUnflaggedFrames(bool state) {
			p_frameList->Freeze();

			if (state) {
				for (int i = 0; i < frameCount; i++) {

					auto frame = frameWidgets->at(i);
					if (frame->IsFlagged()) {
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

		void OnThresholdChange(wxCommandEvent &event) {
			threshold = spn_thresh->GetValue() / 100;

			// if (threshTimer->now() - threshLastUpdated < std::chrono::milliseconds(500)) {
			// 	return;
			// }

			threshLastUpdated = threshTimer->now();
			UpdateFrameFlags();
		}

		void OnOnlyFlaggedChange(wxCommandEvent &event) {
			hideUnflagged = chk_showOnlyFlagged->IsChecked();

			HideUnflaggedFrames(hideUnflagged);
		}

};