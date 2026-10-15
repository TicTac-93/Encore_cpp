#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/dc.h>
#include <wx/generic/statbmpg.h>

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
			frameNum = -1;
			diffLast = 0.0f;
			diffNext = 0.0f;
			isFlagged = false;


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
			// TODO: Need to normalize width of frame gutter to avoid misalignment at 10, 100, etc
			frameNum = number;
			std::string padded = std::to_string(number);
			while (padded.length() < padding) {
				padded = "0" + padded;
			}

			lbl_frameNum->SetLabel(padded);
		}

		void SetName(std::string value) {
			lbl_elapsed->SetLabel(value);
		}

	private:
		int frameNum;
		float diffLast, diffNext;
		bool isFlagged;

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
			lbl_thresh = new wxStaticText(this, -1, "Detection Threshold:");
			spn_thresh = new wxSpinCtrlDouble(this, ID_SPN_THRESH, wxEmptyString,
				wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT,
				0.0, 1.0, 0.0, 0.01);
			chk_autoThresh = new wxCheckBox(this, ID_CHK_AUTOTHRESH,
				wxT("Auto"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);

			vb_frameList = new wxBoxSizer(wxVERTICAL);
			p_frameList = new wxScrolled<wxPanel>(this);
			// p_frameList->SetBackgroundColour(wxColor("#aaaaaa"));
			p_frameList->SetSizer(vb_frameList);
			p_frameList->SetScrollRate(10, 10);

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
		}

		void UpdateCountLabel() {
			lbl_frameCount->SetLabel(std::format("{} Frames ({} Flagged)", frameCount, flaggedCount));
		}

		void SetFrameCount(int count) {
			frameCount = count;
			frameWidgets->clear();
			frameWidgets->reserve(count);
			frameThumbs->clear();
			frameThumbs->reserve(count);
			UpdateCountLabel();
		}

		int GetFrameCount() {
			return frameCount;
		}

		FrameWidget* AddFrame(int index) {
			wxBitmap *bmp_last, *bmp_next;
			if (index > 0) {
				bmp_last = frameThumbs->at(index - 1);
			} else {
				bmp_last = bmp_temp;
			}

			if (index + 1 < frameGroup->GetCount()) {
				bmp_next = frameThumbs->at(index + 1);
			} else {
				bmp_next = bmp_temp;
			}

			auto *newFrame = new FrameWidget(p_frameList, -1, bmp_last, frameThumbs->at(index), bmp_next);
			newFrame->SetFrameNum(index, framePadding);
			frameWidgets->push_back(newFrame);
			vb_frameList->Add(newFrame, 0, wxEXPAND);

			return newFrame;
		}

		void SetFrameGroup(FrameGroup *value) {
			p_frameList->DestroyChildren();
			
			frameGroup = value;
			SetFrameCount(frameGroup->GetCount());
			framePadding = std::to_string(frameCount).length();

			// auto timer = std::chrono::high_resolution_clock();
			// auto time_start = timer.now();

			// Process images in the group
			// For now, just load the images as thumbnails.
			for (int i = 0; i < frameGroup->GetCount(); i++) {
				// Our ImageData is memory hungry!
				ImageData *imgData = LoadImageEXR(frameGroup->GetFrameAt(i)->GetFilepath(), 4);
				frameData->push_back(imgData);
				frameThumbs->push_back(imgData->CreateBitmap());
			}

			p_frameList->Freeze();
			for (int i = 0; i < frameGroup->GetCount(); i++) {
				auto tgtFramePath = frameGroup->GetFrameAt(i);
				auto newFrame = AddFrame(tgtFramePath->GetIndex());
				// Style frame widget
				newFrame->SetName(tgtFramePath->GetFilepath().stem().string());
			}

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
		std::vector<FrameWidget*> *frameWidgets = new std::vector<FrameWidget*>;
		std::vector<ImageData*> *frameData = new std::vector<ImageData*>;
		std::vector<wxBitmap*> *frameThumbs = new std::vector<wxBitmap*>;
		FrameGroup *frameGroup = nullptr;
};