#include "FileListCtrl.h"

wxString FileListCtrl::wxListCtrl::OnGetItemText(long item, long column) const {
	// We can simplify this, since we only deal with a single column
	return itemList->Item(item);
}