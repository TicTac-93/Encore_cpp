#include <wx/wx.h>
#include "EncoreEnums.h"

class MainMenu : public wxMenuBar {
	public:
		MainMenu() {
			file = new wxMenu;
			file->Append(ID_MENU_DIR, wxT("&Directory Selection"));
			file->Append(ID_MENU_SHOT, wxT("&Frame Analysis"));

			file->AppendSeparator();

			file->Append(wxID_EXIT, wxT("&Quit\tCtrl+W"));

			this->Append(file, wxT("&File"));
		}

		wxMenuBar *menubar;
		wxMenu *file;
};