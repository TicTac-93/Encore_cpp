#pragma once

#include <wx/image.h>
#include <wx/bitmap.h>

#include <ImfRgbaFile.h>
#include <ImfRgba.h>
#include <ImfArray.h>

#include <filesystem>
#include <mutex>
#include "SequenceImporter.h"

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
/// @param downsample_factor Optional, the factor to downsample the image by.  2 = half res, 4 = quarter, etc.  Default is 1 (full res).
/// @return A new ImageData object, owned by the caller.
ImageData* LoadImageEXR(path image_path, int downsample_factor);

/// @brief Loads a signle image and stores it at loaded_images[index].  Used in multi-threading.
/// @param tgt_image Path to the image to load.
/// @param index Index of the image in the FrameGroup and loaded_images vector.
/// @param loader Function to load the image with.
/// @param loaded_images A vector of ImageData to store loaded images.
/// @param mtx_loaded_images Mutex to lock the loaded_images vector.
/// @param downsample_factor Passed to loader to control downsampling.
void LoadImageThread(
	path tgt_image,
	int index,
	ImageData* (*loader)(path, int),
	std::vector<ImageData*> *loaded_images,
	std::mutex *mtx_loaded_images,
	int downsample_factor);

/// @brief Attempts to load all image files in the given FrameGroup with multi-threading
/// @param frame_group The FrameGroup to load
/// @param loaded_images An initialized vector of ImageData pointers, to receive the loaded images
/// @param downsample_factor Optional, the factor to downsample images by.  2 = half res, 4 = quarter, etc.  Default is 1 (full res)
/// @return Error code (0 = no error).  Image data can be accessed via the vector passed to loaded_images.
int LoadImageGroup(
	FrameGroup& frame_group,
	std::vector<ImageData*>& loaded_images,
	int downsample_factor);

}