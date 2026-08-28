#include <wx/wx.h>
#include <wx/notebook.h>
#include "Menus.h"
#include "panels/Page_DirectorySelection.h"
#include "panels/Page_FrameAnalysis.h"

// #include "../resources/encore_icon.xpm"

/// @brief The main window constructor for Encore, specific pages will be placed in here
class EncoreFrame : public wxFrame {
	public:
		EncoreFrame()
			: wxFrame(nullptr, wxID_ANY, "Encore", wxDefaultPosition, wxSize(300, 400))
		{
			SetIcon(wxIcon("encore_icon"));

			// Construction
			nb_main = new wxNotebook(this, -1);
			// nb_main->SetBackgroundColour(wxColor(wxT("#cc5555")));
			menu = new MainMenu();

			p_dirSelect = new DirSelectPanel(nb_main);
			nb_main->AddPage(p_dirSelect, "Select Shots");

			p_frameAnalysis = new FrameAnalysisPanel(nb_main);
			nb_main->AddPage(p_frameAnalysis, "Review Frames");

			// Layout
			// auto *layout = new wxBoxSizer(wxVERTICAL);
			// layout->Add(p_dirSelect, 1, wxEXPAND);

			// nb_main->SetSizer(layout);
			SetMenuBar(menu); // Breaks initial layout if added before the sizer

			// Connections
			Bind(wxEVT_MENU, &EncoreFrame::OnQuit, this, wxID_EXIT); // Binds to a method of this class
			// Bind(wxEVT_BUTTON, RightPanel::Add, this->m_rp, ID_PLUS); // Binds to a method of a child
			// Bind(wxEVT_BUTTON, RightPanel::Subtract, this->m_rp, ID_MINUS);

			Center();
		}

	private:
		wxMenuBar *menu;
		wxNotebook *nb_main;
		DirSelectPanel *p_dirSelect;
		FrameAnalysisPanel *p_frameAnalysis;

		void OnQuit(wxCommandEvent &event) {
			Close(true);
		}
};