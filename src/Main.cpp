#include <wx/wx.h>


class MyFrame : public wxFrame {
	public:
		MyFrame() : wxFrame(nullptr, wxID_ANY,  "Hello World") {
			auto* menuFile = new wxMenu;
			menuFile->Append(ID_HELLO,
				"&Hello...\tCtrl+H",
				"Help string shown in status bar for this menu item");
			menuFile->AppendSeparator();
			menuFile->Append(wxID_EXIT);

			auto* menuHelp = new wxMenu;
			menuHelp->Append(wxID_ABOUT);

			auto* menuBar = new wxMenuBar;
			menuBar->Append(menuFile, "&File");
			menuBar->Append(menuHelp, "&Help");

			SetMenuBar(menuBar);

			CreateStatusBar();
			SetStatusText("Welcome to wxWidgets!");

			Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_HELLO);
			Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
			Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
		}

	private:
		void OnHello(wxCommandEvent& event) {
			wxLogMessage("Hello World from wxWidgets!");
		}

		void OnExit(wxCommandEvent& event) {
			Close(true);
		}

		void OnAbout(wxCommandEvent& event) {
			wxMessageBox("This is a wxWidgets Hello World example",
				"About Hello World", wxOK | wxICON_INFORMATION);
		}

		enum {
			ID_HELLO = 1
		};
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