#include <wx/wx.h>

class MainMenu : public wxMenuBar {
	public:
		MainMenu();

		wxMenuBar *menubar;
		wxMenu *file, *import;
};