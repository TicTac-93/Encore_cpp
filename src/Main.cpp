#include <wx/wx.h>
#include "Panels.h"
#include "Menus.h"

#include "../resources/encore_icon.xpm"

class MyFrame : public wxFrame {
	public:
		MyFrame()
			: wxFrame(nullptr, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(300, 400))
		{
			SetIcon(encoreIcon);
			// Construction
			m_parent = new wxPanel(this, -1);
			m_parent->SetBackgroundColour(wxColor(wxT("#cc5555")));
			menu = new MainMenu();
			m_lp = new LeftPanel(m_parent);
			m_rp = new RightPanel(m_parent);
			m_lower = new LowerPanel(m_parent);
			m_grid = new GridPanel(m_parent);

			btn_ok = new wxButton(m_parent, -1, wxT("OK"));
			btn_cancel = new wxButton(m_parent, -1, wxT("Cancel"));

			// Layout
			auto *vbox_main = new wxBoxSizer(wxVERTICAL);

			auto *hbox_1 = new wxBoxSizer(wxHORIZONTAL);
			hbox_1->Add(m_lp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
			hbox_1->Add(m_rp, 1, wxEXPAND | wxALL, 5);

			auto *hbox_2 = new wxBoxSizer(wxHORIZONTAL);
			hbox_2->Add(btn_ok, 0);
			hbox_2->Add(btn_cancel, 0, wxLEFT, 5);

			vbox_main->Add(-1, 10);
			vbox_main->Add(hbox_1, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
			vbox_main->Add(-1, 10);
			vbox_main->Add(m_lower, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
			vbox_main->Add(-1, 10);
			vbox_main->Add(hbox_2, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT, 10);
			vbox_main->Add(-1, 10);
			vbox_main->Add(m_grid, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
			vbox_main->Add(-1, 10);

			m_parent->SetSizer(vbox_main);
			// vbox_main->Fit(this);
			this->SetMenuBar(menu); // Breaks initial layout if added before the sizer
			this->Center();

			// Connections
			Bind(wxEVT_MENU, MyFrame::OnQuit, this, wxID_EXIT); // Binds to a method of this class
			Bind(wxEVT_BUTTON, RightPanel::Add, this->m_rp, ID_PLUS); // Binds to a method of a child
			Bind(wxEVT_BUTTON, RightPanel::Subtract, this->m_rp, ID_MINUS);
		}

	private:
		wxMenuBar *menu;
		wxToolBar *toolbar;
		wxPanel *m_parent;

		LeftPanel *m_lp;
		RightPanel *m_rp;
		LowerPanel *m_lower;
		GridPanel *m_grid;

		wxButton *btn_ok, *btn_cancel;

		void OnQuit(wxCommandEvent &event) {
			Close(true);
		}
};

class MyApp : public wxApp {
	public:
		bool OnInit() override {
			MyFrame* frame = new MyFrame();
			frame->Show();
			return true;
		}
};

wxIMPLEMENT_APP(MyApp);