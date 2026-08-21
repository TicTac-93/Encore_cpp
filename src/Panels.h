#include <wx/wx.h>

class LeftPanel : public wxPanel {
	public:
		LeftPanel(wxPanel *parent);

	private:
		wxPanel *m_parent;
		wxButton *m_plus, *m_minus;
};

class RightPanel : public wxPanel {
	public:
		RightPanel(wxPanel *parent);

		void Add(wxCommandEvent &event);
		void Subtract(wxCommandEvent &event);

	private:
		wxStaticText *m_text;
		int count;

		void SetText(wxString text);
};

class LowerPanel : public wxPanel {
	public:
		LowerPanel(wxPanel *parent);

		wxTextCtrl *textctrl;

};

class GridPanel : public wxPanel {
	public:
		GridPanel(wxPanel *parent);

	private:
		wxButton *b_cls, *b_bck, *b_plus, *b_minus, *b_mul, *b_div, *b_equal,
			*b_pnt, *b_0, *b_1, *b_2, *b_3, *b_4, *b_5, *b_6, *b_7, *b_8, *b_9;
		
		wxTextCtrl *display;

		double value_mem, value_entry;
		bool is_decimal_set;
		int pending_op;

		void input_digit(wxCommandEvent &event);
		void input_op(wxCommandEvent &event);
		void do_op();
};

enum {
	ID_PLUS = 101,
	ID_MINUS = 102,
	ID_MUL = 103,
	ID_DIV = 104,
	ID_EQUALS = 105,
	ID_0 = 106,
	ID_1 = 107,
	ID_2 = 108,
	ID_3 = 109,
	ID_4 = 110,
	ID_5 = 111,
	ID_6 = 112,
	ID_7 = 113,
	ID_8 = 114,
	ID_9 = 115,
	ID_PNT = 116,
	ID_BACK = 117,
	ID_CLS = 118
};