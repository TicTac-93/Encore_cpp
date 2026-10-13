#pragma once

#include <wx/image.h>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>
#include <ImfArray.h>

#include <filesystem>
#include <ctype.h>

using namespace std::filesystem;
using namespace OPENEXR_IMF_NAMESPACE;

namespace encore {

/// @brief Loads an EXR image from disk
/// @param image_path The image to load
/// @param downsample Optional, the factor to downsample the image by.  2 = half res, 4 = quarter, etc.
/// @return 
wxImage* LoadImageEXR(path image_path, int downsample);

}