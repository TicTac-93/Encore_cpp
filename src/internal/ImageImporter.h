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
		float r, g, b;

		Rgb();

		Rgb(float r, float g, float b);

		Rgb operator-(Rgb other);

		operator float();

	private:
};

class ImageData {
	public:
		// Public vars
		float a_mean = 0.f;
		float b_mean = 0.f;
		bool is_loaded = false;
		bool a_set = false;
		bool b_set = false;

		// Constructors
		ImageData();

		ImageData(int width, int height);

		~ImageData();

		// Public member functions
		int GetWidth();

		int GetHeight();

		Rgb &Pixel(int x, int y);

		void SetSize(int width, int height);

		/// @brief Generates a wxBitmap from the image data stored internally,
		///        with optional gamma correction.
		/// @return A wxBitmap, ready for display in the GUI.  Stored internally.
		wxBitmap* CreateBitmap(float gamma = 1.f);

		/// @return The image's wxBitmap
		wxBitmap* Bitmap();

		/// @brief Deletes the pixel data for this image, useful for freeing memory after analysis is complete
		void ClearData();

	private:
		int _w, _h;
		Rgb *_data = nullptr;
		wxBitmap *_bitmap = nullptr;

		void InitializeData();
};



/// @brief Loads an EXR image from disk
/// @param image_path The image to load
/// @param downsample_factor Optional, the factor to downsample the image by.  2 = half res, 4 = quarter, etc.  Default is 1 (full res).
/// @return A new ImageData object, owned by the caller.
ImageData* LoadImageEXR(path image_path, int downsample_factor);

/// @brief Attempts to load all image files in the given FrameGroup with multi-threading,
///        generating wxBitmaps and analyzing the frames as it goes.
/// @param frame_group The FrameGroup to load
/// @param loaded_images Avector of ImageData pointers, to receive the loaded images.  Will be initialized with nullptrs.
/// @param downsample_factor Optional, the factor to downsample images by.  2 = half res, 4 = quarter, etc.  Default is 1 (full res)
/// @return Error code (0 = no error).  Image data can be accessed via the vector passed to loaded_images.
int LoadAndAnalyzeGroup(
	FrameGroup &frame_group,
	std::vector<ImageData*> &loaded_images,
	int downsample_factor);

/// @brief Loads a signle image and stores it at loaded_images[index].  Used in multi-threading.
/// @param tgt_image Path to the image to load.
/// @param index Index of the image in the FrameGroup and loaded_images vector.
/// @param loader Function to load the image with.
/// @param loaded_images A vector of ImageData to store loaded images.
/// @param mtx_start Mutex that will unlock when we should start loading.
/// @param downsample_factor Passed to loader to control downsampling.
void LoadImageThread(
	path tgt_image,
	int index,
	ImageData* (*loader)(path, int),
	std::vector<ImageData*> *loaded_images,
	std::mutex *mtx_start,
	int downsample_factor);

/// @brief Compares each image to its neighbors, calculating the mean difference per-pixel
/// @param images A vector of ImageData to analyze
/// @param index The target image, will be compared to its neighbors
/// @param mtx_imgdata Mutex to lock access to writing to ImageData
void AnalyzeImageThread(
	std::vector<ImageData*> *images,
	int index,
	std::mutex *mtx_imgdata);

/// @brief Unloads the pixel data from the ImageData, and safely decrements the loaded_counter
///        to allow for the next image in the sequence to be processed.
/// @param images A vector of ImageData to clear
/// @param index The target image
/// @param loaded_counter A reference to the counter int
/// @param mtx_counter Mutex to lock access to the counter int
/// @param mtx_imgdata Mutex to lock access to writing to ImageData
void ClearImageDataThread(
	std::vector<ImageData*> *images,
	int index,
	int *loaded_counter,
	std::mutex *mtx_counter,
	std::mutex *mtx_imgdata
);

}