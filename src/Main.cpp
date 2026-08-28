#include <wx/wx.h>
#include "Frame_Main.h"

class MyApp : public wxApp {
	public:
		bool OnInit() override {
			EncoreFrame* frame = new EncoreFrame();
			frame->Show();
			return true;
		}
};

wxIMPLEMENT_APP(MyApp);