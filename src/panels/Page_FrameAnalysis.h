#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "../EncoreEnums.h"

class FrameAnalysisPanel : public wxPanel {
	public:
		FrameAnalysisPanel(wxWindow *parent)
			: wxPanel(parent)
		{
			// Construction
			str_frameCount = new wxString("120 Frames (8 Flagged)");
			lbl_frameCount = new wxStaticText(this, -1, *str_frameCount);
			chk_showOnlyFlagged = new wxCheckBox(this, ID_CHK_ONLYFLAGGED,
				wxT("Only show Flagged frames"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
			lbl_thresh = new wxStaticText(this, -1, "Detection Threshold:");
			spn_thresh = new wxSpinCtrlDouble(this, ID_SPN_THRESH, wxEmptyString,
				wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT,
				0.0, 1.0, 0.0, 0.01);
			chk_autoThresh = new wxCheckBox(this, ID_CHK_AUTOTHRESH,
				wxT("Auto"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);


			// Layout
			auto *layout = new wxBoxSizer(wxVERTICAL);
			auto *layout_outer = new wxBoxSizer(wxHORIZONTAL);

			auto *hbox_top = new wxBoxSizer(wxHORIZONTAL);
			hbox_top->Add(chk_showOnlyFlagged, 0, wxALIGN_CENTER);
			hbox_top->AddStretchSpacer();
			hbox_top->Add(lbl_thresh, 0, wxALIGN_CENTER);
			hbox_top->Add(spn_thresh, 0);
			hbox_top->Add(chk_autoThresh, 0, wxALIGN_CENTER);

			layout->Add(lbl_frameCount, 0, wxEXPAND);
			layout->Add(hbox_top, 0, wxEXPAND);
			layout->AddStretchSpacer();

			layout_outer->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);
			SetSizer(layout_outer);
			layout_outer->Fit(this);


			// Connections
		}

	private:
		wxString *str_frameCount;
		wxStaticText *lbl_frameCount;
		wxCheckBox *chk_showOnlyFlagged;
		wxStaticText *lbl_thresh;
		wxCheckBox *chk_autoThresh;
		wxSpinCtrlDouble *spn_thresh;
		
};