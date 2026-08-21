#include "Menus.h"
#include <wx/file.h>
#include <wx/wfstream.h>

MainMenu::MainMenu()
	: wxMenuBar()
{
	file = new wxMenu;
	file->Append(wxID_ANY, wxT("&New"));
	file->Append(wxID_ANY, wxT("&Open"));
	file->Append(wxID_EXIT, wxT("&Save"));
	file->AppendSeparator();

	import = new wxMenu;
	import->Append(wxID_ANY, wxT("Import newsfeed..."));
	import->Append(wxID_ANY, wxT("Import bookmarks..."));
	import->Append(wxID_ANY, wxT("Import mail..."));
	file->AppendSubMenu(import, wxT("&Import"));

	file->Append(wxID_EXIT, wxT("&Quit\tCtrl+W"));

	this->Append(file, wxT("&File"));
}