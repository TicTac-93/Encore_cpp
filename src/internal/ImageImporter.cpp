#include "ImageImporter.h"
#include <Imath/ImathVec.h>
#include <thread>
// #include <ctype.h>

using namespace encore;
using namespace Imath;

const int THREAD_SLEEP_DUR = 10;


// ============================================================================
// CLASS Rgb

Rgb::Rgb()
:	r(0.f),
	g(0.f),
	b(0.f)
{

}

Rgb::Rgb(float r, float g, float b)
:	r(r),
	g(g),
	b(b)
{

}

Rgb Rgb::operator-(Rgb other) {
	return Rgb(
		r - other.r,
		g - other.g,
		b - other.b);
}

Rgb::operator float() {
	return (r + g + b) / 3.f;
}


// ============================================================================
// CLASS ImageData

ImageData::ImageData() 
:	_w(0),
	_h(0)
{
}

ImageData::ImageData(int size_x, int size_y) 
:	_w(size_x),
	_h(size_y)
{
	InitializeData();
}

ImageData::~ImageData() {
	delete[] _data;
	delete _bitmap;
}

int ImageData::GetWidth() {
	return _w;
}

int ImageData::GetHeight() {
	return _h;
}

void ImageData::SetSize(int size_x, int size_y) {
	_w = size_x;
	_h = size_y;

	InitializeData();
}

Rgb& ImageData::Pixel(int x, int y) {
	if (x < 0
	|| x > _w
	|| y < 0
	|| y > _h) {
		throw(std::range_error("Attempting to access invalid pixel!"));
	}

	return _data[(y * _w) + x];
}

wxBitmap* ImageData::CreateBitmap(float gamma) {
	int output_size = (sizeof(unsigned char) * _w * _h * 3);
	unsigned char *output_data = (unsigned char*)malloc(output_size);
	long int offset = 0;
	int incr = sizeof(unsigned char);

	auto image = wxImage(wxSize(_w, _h));

	for (int y = 0; y < _h; y++) {
		for (int x = 0; x < _w; x++) {
			Rgb pixel = Pixel(x, y);
			output_data[offset]     = std::clamp((int)(std::powf(pixel.r, gamma) * 255.f), 0, 255);
			output_data[offset + 1] = std::clamp((int)(std::powf(pixel.g, gamma) * 255.f), 0, 255);
			output_data[offset + 2] = std::clamp((int)(std::powf(pixel.b, gamma) * 255.f), 0, 255);

			offset += 3;
		}
	}

	image.SetData(output_data);
	if (_bitmap != nullptr) {
		delete _bitmap;
	}
	_bitmap = new wxBitmap(image);
	return _bitmap;
}

wxBitmap* ImageData::Bitmap() {
	return _bitmap;
}

void ImageData::ClearData() {
	if (_data != nullptr) {
		delete[] _data;
	}

	_data = nullptr;
}

void ImageData::InitializeData() {
	if (_data != nullptr) {
		delete[] _data;
	}

	_data = new Rgb[_w * _h];
	for (int i = 0; i < (_w * _h); i++) {
		_data[i] = Rgb();
	}
}


// ============================================================================
// FUNCTIONS

ImageData* encore::LoadImageEXR(path image_path, int downsample_factor = 1) {
	// Clamp downsample_factor
	if (downsample_factor < 1) {
		downsample_factor = 1;
	}

	// EXR loading
	RgbaInputFile file(image_path.filename().string().c_str());

	Box2i dw = file.dataWindow();
	int width = dw.max.x - dw.min.x + 1;
	int height = dw.max.y - dw.min.y + 1;

	Array2D<Rgba> pixels = Array2D<Rgba>(height, width);

	file.setFrameBuffer(&pixels[0][0], 1, width);
	file.readPixels(dw.min.y, dw.max.y);
	
	int output_w = width / downsample_factor;
	int output_h = height / downsample_factor;
	ImageData* output = new ImageData(output_w, output_h);
	for (int y1 = 0, y2 = 0; y1 < height; y1 += downsample_factor, y2++) {
		for (int x1 = 0, x2 = 0; x1 < width; x1 += downsample_factor, x2++) {
			Rgba pixel = pixels[y1][x1]; // EXR pixel
			Rgb &out_px = output->Pixel(x2, y2); // ImageData pixel
			out_px.r = pixel.r;
			out_px.g = pixel.g;
			out_px.b = pixel.b;
		}
	}

	return output;
}

int encore::LoadAndAnalyzeGroup(
	FrameGroup& frame_group,
	std::vector<ImageData*> &loaded_images,
	int downsample_factor = 1)
{
	const int LOAD_LIMIT = 10;

	int frame_count = frame_group.GetCount();
	int *loaded_count = new int(0);
	std::thread *loading_threads[frame_count];
	std::thread *analysis_threads[frame_count];
	std::thread *clear_threads[frame_count];
	std::mutex *mtx_start[frame_count];
	std::mutex *mtx_imgdata = new std::mutex();
	std::mutex *mtx_loaded_counter = new std::mutex();

	// Pre-populate the mutex arrays
	for (int i = 0; i < frame_count; i++) {
		mtx_start[i] = new std::mutex();
		mtx_start[i]->lock();
	}

	// Pre-populate the loaded_images vector with nullptrs,
	// as well as our thread arrays
	for (int i = 0; i < frame_count; i++) {
		loaded_images[i] = nullptr;
		loading_threads[i] = nullptr;
		analysis_threads[i] = nullptr;
		clear_threads[i] = nullptr;
	}

	// Loading is done sequentially, with a limited number of concurrent threads
	// to reduce memory overhead.
	// Each ImageData has a "loaded" property that is TRUE once the image is loaded
	// and saved as a wxBitmap.

	// Another set of threads to ANALYZE every frame after loading, comparing it to its neighbors.
	// Each ImageData has "a_set" and "b_set" properties that are TRUE once they've been compared
	// w/ their neighboring frames.  When BOTH are TRUE, the image is considered analyzed.

	// A final set of threads CLEARS each image's data once its loaded and x_set properties are TRUE.
	// This also makes room in the LOADING queue.


	// Create LOAD threads based on filetype
	if (frame_group.GetFiletype() == ".exr") {
		for (int i = 0; i < frame_count; i++) {
			auto loader_thread = new std::thread(
				LoadImageThread,
				frame_group.GetFrameAt(i)->GetFilepath(),
				i,
				&encore::LoadImageEXR,
				&loaded_images,
				mtx_start[i],
				downsample_factor);

			loading_threads[i] = loader_thread;
		}
	}
	else {
		return 1;
	}

	// Create ANALYZE and CLEAR threads, which will block until they're able to acquire their mutex.
	for (int i = 0; i < frame_count; i++) {
		// We only have to analyze every-other image, sharing the data with neighbors
		if (i % 2 == 0) {
			auto analysis_thread = new std::thread(
				AnalyzeImageThread,
				&loaded_images,
				i,
				mtx_imgdata);
			analysis_threads[i] = analysis_thread;
		}

		auto clear_thread = new std::thread(
			ClearImageDataThread,
			&loaded_images,
			i,
			loaded_count,
			mtx_loaded_counter,
			mtx_imgdata);
		clear_threads[i] = clear_thread;
	}

	// Unlock load threads sequentially, until LOAD_LIMIT is reached
	for (int i = 0; i < frame_count; i++) {
		while ((*loaded_count) >= LOAD_LIMIT) {
			Sleep(THREAD_SLEEP_DUR);
		}
		mtx_loaded_counter->lock();
			(*loaded_count) ++;
		mtx_loaded_counter->unlock();

		mtx_start[i]->unlock();
	}

	// Wait for all threads to exit
	for (int i = 0; i < frame_count; i++) {
		loading_threads[i]->join();
		if (analysis_threads[i] != nullptr) {
			analysis_threads[i]->join();
		}
		clear_threads[i]->join();
	}

	return 0;
}

void encore::LoadImageThread(
	path tgt_image,
	int index,
	ImageData* (*loader)(path, int),
	std::vector<ImageData*> *loaded_images,
	std::mutex *mtx_start,
	int downsample_factor)
{
	// This will block until the calling thread unlocks it, to limit concurrent image loads.
	mtx_start->lock();

	ImageData *imgData = loader(tgt_image, downsample_factor);
	imgData->CreateBitmap(.45f);
	imgData->is_loaded = true;
	loaded_images->at(index) = imgData;
}

void encore::AnalyzeImageThread(
	std::vector<ImageData*> *images,
	int index,
	std::mutex *mtx_imgdata)
{
	ImageData *img, *img_a, *img_b;
	
	// Get this and neighboring images, checking is_loaded prop to wait for initialization.
	// If we're a the edge of the frame range, just use this instead of a or b.
	while ( (img = images->at(index)) == nullptr || !img->is_loaded ) {
		Sleep(THREAD_SLEEP_DUR);
	}

	if (index > 0) {
		while ( (img_a = images->at(index-1)) == nullptr || !img_a->is_loaded ) {
			Sleep(THREAD_SLEEP_DUR);
		}
	} else {
		img_a = img;
	}

	if (index + 1 < images->size()) {
		while ( (img_b = images->at(index+1)) == nullptr || !img_b->is_loaded ) {
			Sleep(THREAD_SLEEP_DUR);
		}
	} else {
		img_b = img;
	}
	
	float total_diff_a = 0.f;
	float total_diff_b = 0.f;
	// Accumulate differences per-pixel
	for (int y = 0; y < img->GetHeight(); y++) {
		for (int x = 0; x < img->GetWidth(); x++) {
			float diff_a = img->Pixel(x, y) - img_a->Pixel(x, y);
			float diff_b = img->Pixel(x, y) - img_b->Pixel(x, y);
			
			if (diff_a < 0.f) {
				diff_a = -diff_a;
			}
			if (diff_b < 0.f) {
				diff_b = -diff_b;
			}

			total_diff_a += diff_a;
			total_diff_b += diff_b;
		}
	}

	// Convert to mean and store
	int pixel_count = img->GetWidth() * img->GetHeight();
	float mean_a = total_diff_a / pixel_count;
	float mean_b = total_diff_b / pixel_count;

	// Limit write access to ImageData to one thread at a time, just to be safe
	mtx_imgdata->lock();
		img->a_mean = mean_a;
		img->a_set = true;
		img->b_mean = mean_b;
		img->b_set = true;

		// Set values on adjacent frame
		// These will be skipped if we're at the edge of the range, ie img_a==img
		if (!img_a->b_set) {
			img_a->b_mean = mean_a;
			img_a->b_set = true;
		}
		if (!img_b->a_set) {
			img_b->a_mean = mean_b;
			img_b->a_set = true;
		}
	mtx_imgdata->unlock();
}

void encore::ClearImageDataThread(
	std::vector<ImageData*> *images,
	int index,
	int *loaded_counter,
	std::mutex *mtx_counter,
	std::mutex *mtx_imgdata
)
{
	ImageData* img;

	// Periodically check if the image has been initialized
	while ( (img = images->at(index)) == nullptr ) {
		Sleep(THREAD_SLEEP_DUR);
	}

	// Then wait for it to be loaded and analyzed
	while ( !img->is_loaded
	|| !img->a_set
	|| !img->b_set) {
		Sleep(THREAD_SLEEP_DUR);
	}

	// Clear the image data and decrement the loaded counter
	mtx_imgdata->lock();
		img->ClearData();
		img->is_loaded = false;
	mtx_imgdata->unlock();

	mtx_counter->lock();
		(*loaded_counter) --;
	mtx_counter->unlock();
}