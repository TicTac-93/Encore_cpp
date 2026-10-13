#include "SequenceImporter.h"
#include <Imath/ImathVec.h>

using namespace encore;
using namespace Imath;


// ============================================================================
// CLASS FramePath

FramePath::FramePath(path *frame) {
	filepath = frame;
	// TODO: UTF(8?) support
	filename = new std::string(filepath->stem().string());
	filetype = new std::string(filepath->extension().string());

	// Extract frame index from filename
	// For now, we only support trailing frame numbers
	std::string index_str = "";
	bool cont = true;
	for (int i = filename->length() - 1; cont && i >= 0; i--) {
		char character = filename->at(i);
		if (isdigit(character)) {
			// Remember, we're reading this right-to-left
			index_str = character + index_str;
		}
		else {
			cont = false;
			// Update filename with numbers removed
			*filename = filename->substr(0, i+1);
		}
	}

	if (index_str.length() > 0) {
		index = new int(std::stoi(index_str));
	}
	else {
		index = new int(-1);
	}
}

path FramePath::GetFilepath() {
	return *filepath;
}

std::string FramePath::GetName() {
	return *filename;
}

std::string FramePath::GetType() {
	return *filetype;
}

int FramePath::GetIndex() {
	return *index;
}


// ============================================================================
// CLASS FrameGroup

FrameGroup::FrameGroup() {
	frames = new std::vector<FramePath *>();
	filename = new std::string();
	filetype = new std::string();
	index_min = new int(0);
	index_max = new int(0);
	count = new int(0);
}

FrameGroup::FrameGroup(FramePath *frame) {
	frames = new std::vector<FramePath *>();
	frames->push_back(frame);
	filename = new std::string(frame->GetName());
	filetype = new std::string(frame->GetType());
	index_min = new int(frame->GetIndex());
	index_max = new int(*index_min);
	count = new int(1);
}

void FrameGroup::AddFrame(FramePath *frame) {
	frames->push_back(frame);
	(*count) ++;
	int new_index = frame->GetIndex();
	if (new_index < *index_min) {
		*index_min = new_index;
	}
	else if (new_index > *index_max) {
		*index_max = new_index;
	}
}

int FrameGroup::GetMin() {
	return *index_min;
}

void FrameGroup::SetMin(int value) {
	*index_min = value;
}

int FrameGroup::GetMax() {
	return *index_max;
}

void FrameGroup::SetMax(int value) {
	*index_max = value;
}

int FrameGroup::GetCount() {
	return *count;
}

std::string FrameGroup::GetFiletype() {
	return *filetype;
}

void FrameGroup::SetFiletype(std::string value) {
	*filetype = value;
}

std::string FrameGroup::GetFilename() {
	return *filename;
}

void FrameGroup::SetFilename(std::string value) {
	*filename = value;
}

FramePath* FrameGroup::GetFrameAt(int index) {
	if (index >= 0 && index < frames->size()) {
		return frames->at(index);
	}
	else {
		return nullptr;
	}
}


// ============================================================================
// FUNCTIONS

int encore::GetFilesInDir(wxString path, wxArrayString *filesArray) {
	wxDir dir = wxDir();
	// Confirm the given path is valid
	if (dir.Open(path)) {
		dir.GetAllFiles(path, filesArray, wxEmptyString, wxDIR_FILES);
		dir.Close();
		return 0;
	}
	else {
		return 1;
	}
}

std::vector<FrameGroup *>* encore::GroupFrameSequences(wxArrayString *filesArray) {
	auto groups = new std::vector<FrameGroup *>();
	for (int i = 0; i < filesArray->GetCount(); i++) {
		bool found_group = false;
		path *filepath = new path(filesArray->Item(i).ToStdString());

		// Skip unsupported file formats
		if (filepath->extension() != ".exr") {
			continue;
		}

		auto frame = new FramePath(filepath);
		
		// Check against existing groups
		auto name = frame->GetName();
		for (int i = 0; i < groups->size(); i++) {
			FrameGroup *group = groups->at(i);
			if (name == group->GetFilename()) {
				group->AddFrame(frame);
				found_group = true;
				break;
			}
		}

		if (!found_group) {
			groups->push_back(new FrameGroup(frame));
		}
	}

	return groups;
}