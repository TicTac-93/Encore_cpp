#include <wx/wx.h>
#include <wx/dir.h>
#include <filesystem>
#include <ctype.h>

using namespace std::filesystem;

namespace encore {

/// @brief A class for storing a path to a single frame, with pre-processed metadata.
class FramePath {
	public:
		FramePath(path *frame);

		/// @brief Returns the full filepath to this frame
		path GetFilepath();

		/// @brief Returns the frame's filename, without the number or extension.
		std::string GetName();

		/// @brief Returns the frame's filetype in the form of its extension.
		std::string GetType();

		/// @brief Returns the frame's calculated index
		int GetIndex();

	private:
		path *filepath;  // The full path to this frame, on disk
		std::string *filename;  // The filename, without trailing numbers or extension
		std::string *filetype;  // The file's extension
		int *index;  // The frame number extracted from the filename
};

/// @brief A group of frames in a sequence, though not necessarily in order.
class FrameGroup {
	public:
		FrameGroup();
		FrameGroup(FramePath *frame);

		/// @brief Adds the given frame to this group, automatically updating
		///        the count and min/max indices using the FramePath's data.
		void AddFrame(FramePath *frame);

		/// @brief Returns the minimum index value in this FrameGroup.
		int GetMin();

		/// @brief Manually set the minimum index in this group.  Has no effect
		///        on the frames stored within.
		void SetMin(int value);

		/// @brief Returns the maximum index value in this FrameGroup.
		int GetMax();

		/// @brief Manually set the maximum index in this group.  Has no effect
		///        on the frames stored within.
		void SetMax(int value);

		/// @brief Returns the number of frames in this group.
		int GetCount();

		/// @brief Returns the file extension for the frames in this group.
		std::string GetFiletype();

		/// @brief Manually set the file extension for this group.
		void SetFiletype(std::string value);

		/// @brief Returns the frame at the given index.
		FramePath GetFrameAt(int index);

		/// @brief Returns the group's filename.
		std::string GetFilename();

		/// @brief Set the group's filename to the given string.
		///        Should be stripped of any numbering and extension.
		void SetFilename(std::string value);

	private:
		std::vector<FramePath *> *frames;
		std::string *filename, *filetype;
		int *index_min, *index_max, *count;
};

/// @brief Uses wxWidgets to fetch all files in a given directory, writing them to the filesArray.
/// @param path Path to the directory to be searched.
/// @param filesArray Pointer to a wxArrayString for storing found file paths.
/// @return 0 on success, 1 if the directory could not be accessed.
int GetFilesInDir(wxString path, wxArrayString *filesArray);

/// @brief Converts an array of string paths to FramePaths, grouping them by name
///        and returning a vector of all groups.
/// @param filesArray 
/// @return 
std::vector<FrameGroup *>& GroupFrameSequences(wxArrayString *filesArray);

}