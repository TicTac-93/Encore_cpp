#pragma once

#include <wx/wx.h>
#include <format>
#include "FileListCtrl.h"
#include "../EncoreEnums.h"
#include "../internal/SequenceImporter.h"

using namespace encore;

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
			lc_filelist = new FileListCtrl(this, ID_LC_FILES, *fileListArray);
			btn_load_frames = new wxButton(this, ID_BTN_LOAD, "Load Frames");
			DisableLoadBtn();
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
			layout->AddStretchSpacer();

			auto *hbox_2 = new wxBoxSizer(wxHORIZONTAL);
			hbox_2->AddStretchSpacer();
			hbox_2->Add(btn_load_frames, 0, wxEXPAND);
			layout->Add(hbox_2, 0, wxEXPAND);

			layout_outer->Add(layout, 1, wxEXPAND | wxALL, PAD_MED);
			SetSizer(layout_outer);
			layout_outer->Fit(this);

			// Connections
			Bind(wxEVT_BUTTON, &DirSelectPanel::OnBrowse, this, ID_BTN_DIR);
			Bind(wxEVT_LIST_ITEM_SELECTED, &DirSelectPanel::OnFrameSelect, this, ID_LC_FILES);
			Bind(wxEVT_LIST_ITEM_DESELECTED, &DirSelectPanel::OnFrameSelect, this, ID_LC_FILES);
		}

		/// @brief Collects and returns FrameGroup objects based on the user's current UI selection.
		/// @return A pointer to a vector of FrameGroups, or nullptr if nothing was selected.
		std::vector<FrameGroup *>* GetSelectedGroups() {
			auto output = new std::vector<FrameGroup *>();
			long list_item = -1;
			// Rely on GetNextItem to traverse the filelist and return -1 to break us out
			for (;;) {
				list_item = lc_filelist->GetNextItem(list_item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
				
				if (list_item == -1) {
					break;
				}

				output->push_back((*frameGroups)[list_item]);
			}

			if (output->size() == 0) {
				return nullptr;
			}
			else {
				return output;
			}
		}

	private:
		wxStaticText *lbl_dir;
		wxTextCtrl *field_dir;
		wxButton *btn_browse_dir;
		wxStaticText *lbl_filelist;
		FileListCtrl *lc_filelist;
		wxArrayString *fileListArray = new wxArrayString();
		std::vector<FrameGroup *> *frameGroups = new std::vector<FrameGroup *>();
		wxButton *btn_load_frames;
		wxColor *itemColorA = new wxColor("#ffffff");
		wxColor *itemColorB = new wxColor("#bbbbbb");

		void HideFilelist() {
			lc_filelist->Hide();
			lbl_filelist->Hide();
			btn_load_frames->Hide();
			this->Layout();
		}

		void ShowFilelist() {
			lc_filelist->Show();
			lc_filelist->RefreshAfterUpdate();
			lbl_filelist->Show();
			btn_load_frames->Show();
			this->Layout();
		}

		void DisableLoadBtn() {
			btn_load_frames->Disable();
			this->Layout();
		}

		void EnableLoadBtn() {
			btn_load_frames->Enable();
			this->Layout();
		}

		void OnBrowse(wxCommandEvent& event) {
			wxDirDialog *browseDialog = new wxDirDialog(this, "Select directory to scan...",
				field_dir->GetValue(), wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR);

			// Execute if the user did not cancel or close the dialog
			if (browseDialog->ShowModal() == wxID_OK) {
				wxString path_selected = browseDialog->GetPath();
				field_dir->SetValue(path_selected);
				auto filesArray = wxArrayString();
				if (GetFilesInDir(path_selected, &filesArray) == 0) {
					if (filesArray.GetCount() == 0) {
						*fileListArray = filesArray;
						HideFilelist();
					} else {
						frameGroups = GroupFrameSequences(&filesArray);
						fileListArray->Clear();
						for (int i = 0; i < frameGroups->size(); i++) {
							auto group = frameGroups->at(i);
							fileListArray->Add(std::format("{}[{}-{}]{}", group->GetFilename(), group->GetMin(), group->GetMax(), group->GetFiletype()));
						}
						ShowFilelist();
					}

				}
			}
		}

		void OnFrameSelect(wxCommandEvent& event) {
			if (lc_filelist->GetSelectedItemCount() > 0) {
				EnableLoadBtn();
			}
			else {
				DisableLoadBtn();
			}
		}

};