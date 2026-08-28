#include <wx/listctrl.h>

class FileListCtrl : public wxListCtrl {
	public:
		FileListCtrl(wxWindow *parent, wxWindowID id, wxArrayString &fileList)
			: wxListCtrl(parent, id, wxDefaultPosition, wxDefaultSize,
				wxLC_VIRTUAL | wxLC_REPORT | wxLC_NO_HEADER, wxDefaultValidator, "")
		{
			itemList = &fileList;
			AppendColumn("Filename");
			EnableAlternateRowColours();
		}

		void SetItemList(wxArrayString &newItemList) {
			itemList = &newItemList;
		}

		wxString OnGetItemText(long item, long column) const override {
			// We can simplify this, since we only deal with a single column
			return itemList->Item(item);
		}

		void RefreshAfterUpdate() {
			SetItemCount(itemList->GetCount());
			SetColumnWidth(0, -1);
			Refresh();
		}


	private:
		wxArrayString *itemList;
};