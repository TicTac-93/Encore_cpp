#include <wx/wx.h>
#include <wx/dir.h>
#include "FileListCtrl.h"
#include "../EncoreEnums.h"

class DirSelectPanel : public wxPanel {
	public:
		DirSelectPanel(wxWindow *parent)
			: wxPanel(parent)
		{
			// Construction
			// SetBackgroundColour(wxColor(wxT("#bbbbbb")));
			lbl_dir = new wxStaticText(this, -1, "Directory to process:");
			field_dir = new wxTextCtrl(this, ID_DIR_FIELD, "",
				wxDefaultPosition, wxDefaultSize,
				wxTE_PROCESS_ENTER);
			btn_browse_dir = new wxButton(this, ID_BTN_DIR, "Browse...");

			lbl_filelist = new wxStaticText(this, -1, "Select Frame Sequences to Analyze:");
			filesArray = new wxArrayString();
			lc_filelist = new FileListCtrl(this, -1, *filesArray);
			HideFilelist();

			// Layout
			auto *layout = new wxBoxSizer(wxVERTICAL);
			auto *layout_outer = new wxBoxSizer(wxHORIZONTAL);

			auto *hbox_1 = new wxBoxSizer(wxHORIZONTAL);
			hbox_1->Add(field_dir, 1, wxEXPAND);
			hbox_1->AddSpacer(PAD_MED);
			hbox_1->Add(btn_browse_dir, 0, 0);
			
			layout->Add(lbl_dir, 0, wxEXPAND);
			layout->Add(hbox_1, 0, wxEXPAND);
			layout->AddSpacer(PAD_MED);
			layout->Add(lbl_filelist, 0, wxEXPAND);
			layout->AddSpacer(PAD_SML);
			layout->Add(lc_filelist, 0, wxEXPAND);

			layout_outer->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);
			SetSizer(layout_outer);
			layout_outer->Fit(this);

			// Connections
			Bind(wxEVT_BUTTON, &DirSelectPanel::OnBrowse, this, ID_BTN_DIR);
		}

	private:
		wxStaticText *lbl_dir;
		wxTextCtrl *field_dir;
		wxButton *btn_browse_dir;
		wxStaticText *lbl_filelist;
		FileListCtrl *lc_filelist;
		wxArrayString *filesArray;
		wxColor *itemColorA = new wxColor("#ffffff");
		wxColor *itemColorB = new wxColor("#bbbbbb");

		void HideFilelist() {
			lc_filelist->Hide();
			lbl_filelist->Hide();
			this->Layout();
		}

		void ShowFilelist() {
			lc_filelist->Show();
			lc_filelist->RefreshAfterUpdate();
			lbl_filelist->Show();
			this->Layout();
		}

		void OnBrowse(wxCommandEvent& event) {
			wxDirDialog *browseDialog = new wxDirDialog(this, "Select directory to scan...",
				field_dir->GetValue(), wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR);

			// Execute if the user did not cancel or close the dialog
			if (browseDialog->ShowModal() == wxID_OK) {
				wxString path_selected = browseDialog->GetPath();
				field_dir->SetValue(path_selected);
				GetFilesInDir(path_selected);
			}
		}

		void GetFilesInDir(wxString path) {
			wxDir dir = wxDir();
			// Confirm the given path is valid
			if (dir.Open(path)) {
				dir.GetAllFiles(path, filesArray, wxEmptyString, wxDIR_FILES);
			}

			if (filesArray->GetCount() == 0) {
				HideFilelist();
			} else {
				ShowFilelist();
			}
		}
};