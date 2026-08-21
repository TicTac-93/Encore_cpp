#include <wx/wx.h>
#include "Panels.h"

LeftPanel::LeftPanel(wxPanel *parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN)
{
	m_parent = parent;
	m_plus = new wxButton(this, ID_PLUS, wxT("+"));
	m_minus = new wxButton(this, ID_MINUS, wxT("-"));

	auto *vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(m_plus);
	vbox->Add(m_minus);
	// vbox->SetSizeHints(this);
	SetSizer(vbox);
	vbox->Fit(this);
}


// =============================================================================
RightPanel::RightPanel(wxPanel *parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN)
{
	count = 0;
	m_text = new wxStaticText(this, -1, std::to_string(count));
}

void RightPanel::Add(wxCommandEvent &event) {
	count ++;
	SetText(std::to_string(count));
}

void RightPanel::Subtract(wxCommandEvent &event) {
	count --;
	SetText(std::to_string(count));
}

void RightPanel::SetText(wxString text) {
	m_text->SetLabel(text);
}


// =============================================================================
LowerPanel::LowerPanel(wxPanel *parent)
	: wxPanel(parent, -1)
{
	// auto *inner = new wxPanel(this, -1);
	// inner->SetBackgroundColour(wxColor(wxT("#111111")));
	textctrl = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition,
		wxDefaultSize, wxTE_MULTILINE);

	auto *vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(textctrl, 1, wxEXPAND);

	SetSizer(vbox);
	// Center();
}


// =============================================================================
GridPanel::GridPanel(wxPanel *parent)
	: wxPanel(parent, -1)
{
	value_mem = value_entry = 0.0f;
	is_decimal_set = false;
	pending_op = 0;


	auto *sizer = new wxBoxSizer(wxVERTICAL);
	display = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT | wxTE_READONLY);

	sizer->Add(display, 0, wxEXPAND);
	sizer->Add(-1, 5);

	auto *grid = new wxGridSizer(5, 4, 3, 3);

	b_cls = new wxButton(this, ID_CLS, wxT("Cls"));
	b_bck = new wxButton(this, ID_BACK, wxT("Back"));
	b_plus = new wxButton(this, ID_PLUS, wxT("+"));
	b_minus = new wxButton(this, ID_MINUS, wxT("-"));
	b_mul = new wxButton(this, ID_MUL, wxT("*"));
	b_div = new wxButton(this, ID_DIV, wxT("/"));
	b_equal = new wxButton(this, ID_EQUALS, wxT("="));
	b_pnt = new wxButton(this, ID_PNT, wxT("."));
	b_0 = new wxButton(this, ID_0, wxT("0"));
	b_1 = new wxButton(this, ID_1, wxT("1"));
	b_2 = new wxButton(this, ID_2, wxT("2"));
	b_3 = new wxButton(this, ID_3, wxT("3"));
	b_4 = new wxButton(this, ID_4, wxT("4"));
	b_5 = new wxButton(this, ID_5, wxT("5"));
	b_6 = new wxButton(this, ID_6, wxT("6"));
	b_7 = new wxButton(this, ID_7, wxT("7"));
	b_8 = new wxButton(this, ID_8, wxT("8"));
	b_9 = new wxButton(this, ID_9, wxT("9"));

	grid->Add(b_cls, 0, wxEXPAND);
	grid->Add(b_bck, 0, wxEXPAND);
	grid->Add(-1, wxEXPAND);
	grid->Add(-1, wxEXPAND);
	grid->Add(b_7, 0, wxEXPAND);
	grid->Add(b_8, 0, wxEXPAND);
	grid->Add(b_9, 0, wxEXPAND);
	grid->Add(b_div, 0, wxEXPAND);
	grid->Add(b_4, 0, wxEXPAND);
	grid->Add(b_5, 0, wxEXPAND);
	grid->Add(b_6, 0, wxEXPAND);
	grid->Add(b_mul, 0, wxEXPAND);
	grid->Add(b_1, 0, wxEXPAND);
	grid->Add(b_2, 0, wxEXPAND);
	grid->Add(b_3, 0, wxEXPAND);
	grid->Add(b_minus, 0, wxEXPAND);
	grid->Add(b_0, 0, wxEXPAND);
	grid->Add(b_pnt, 0, wxEXPAND);
	grid->Add(b_equal, 0, wxEXPAND);
	grid->Add(b_plus, 0, wxEXPAND);

	sizer->Add(grid, 1, wxEXPAND);
	SetSizer(sizer);
	SetMinSize(wxSize(270, 220));

	// Connections
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_0);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_1);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_2);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_3);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_4);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_5);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_6);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_7);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_8);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_9);
	Bind(wxEVT_BUTTON, GridPanel::input_digit, this, ID_PNT);

	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_BACK);
	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_CLS);
	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_EQUALS);
	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_MUL);
	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_DIV);
	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_PLUS);
	Bind(wxEVT_BUTTON, GridPanel::input_op, this, ID_MINUS);
}

void GridPanel::input_digit(wxCommandEvent &event) {
	int id = event.GetId();
	int val = -1;

	switch(id){
		case(ID_0):
			val = 0;
			break;

		case(ID_1):
			val = 1;
			break;

		case(ID_2):
			val = 2;
			break;

		case(ID_3):
			val = 3;
			break;

		case(ID_4):
			val = 4;
			break;
			
		case(ID_5):
			val = 5;
			break;
			
		case(ID_6):
			val = 6;
			break;
			
		case(ID_7):
			val = 7;
			break;
			
		case(ID_8):
			val = 8;
			break;
			
		case(ID_9):
			val = 9;
			break;
			
		case(ID_PNT):
			if (!is_decimal_set) {
				display->AppendText(".");
				is_decimal_set = true;
			}

			break;

		default:
			return;
	}

	if (val != -1 ) {
		// Only update these if we're adding a value.  Decimal point doesn't count.
		display->AppendText(std::to_string(val));
		value_entry = std::stof( (std::string)display->GetValue() );
	}
}

void GridPanel::input_op(wxCommandEvent &event) {
	int id = event.GetId();
	wxString buffer = display->GetValue();

	switch(id) {
		case(ID_BACK):
			buffer.RemoveLast();
			display->SetValue(buffer);
			value_entry = std::stof( (std::string)display->GetValue() );
			break;

		case(ID_CLS):
			display->SetValue("");
			value_entry = 0;
			break;

		case(ID_PLUS):
		case(ID_MINUS):
		case(ID_MUL):
		case(ID_DIV):

			if (pending_op != 0
				&& buffer.length() > 0)
			{
				do_op();
			}
			else {
				pending_op = id;
				value_mem = std::stof( (std::string)buffer );
				display->SetValue("");
			}

			printf("value_mem: %f\n", value_mem);
			break;

		default:
			return;
	}
}

void GridPanel::do_op() {
	switch(pending_op) {
		case(ID_PLUS):
			value_mem = value_mem + value_entry;
			break;

		case(ID_MINUS):
			value_mem = value_mem - value_entry;
			break;

		case(ID_MUL):
			value_mem = value_mem * value_entry;
			break;
		
		case(ID_DIV):
			value_mem = value_mem / value_entry;
			break;

		default:
			pending_op = 0;
			return;
	}

	pending_op = 0;
	value_entry = value_mem;
	display -> SetValue(std::to_string(value_mem));
}