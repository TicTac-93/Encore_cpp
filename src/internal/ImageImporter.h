#pragma once

#include <wx/image.h>
#include <wx/bitmap.h>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>
#include <ImfArray.h>

#include <filesystem>
#include <ctype.h>

using namespace std::filesystem;
using namespace OPENEXR_IMF_NAMESPACE;

namespace encore {

class Rgb {
	public:
		Rgb();

		Rgb(float r, float g, float b);

		float r, g, b;

	private:
};

class ImageData {
	public:
		ImageData();

		ImageData(int width, int height);

		~ImageData();

		int GetWidth();

		int GetHeight();

		Rgb& Pixel(int width, int height);

		void SetSize(int width, int height);

		wxBitmap* CreateBitmap();

	private:
		int _w, _h;
		Rgb *_data;

		void InitializeData();
};

/// @brief Loads an EXR image from disk
/// @param image_path The image to load
/// @param downsample Optional, the factor to downsample the image by.  2 = half res, 4 = quarter, etc.
/// @return 
ImageData* LoadImageEXR(path image_path, int downsample);

}