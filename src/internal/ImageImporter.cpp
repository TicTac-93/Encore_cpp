#include "ImageImporter.h"
#include <Imath/ImathVec.h>

using namespace encore;
using namespace Imath;


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


// ============================================================================
// CLASS ImageData

ImageData::ImageData() 
:	_w(0),
	_h(0)
{
	_data = nullptr;
}

ImageData::ImageData(int size_x, int size_y) 
:	_w(size_x),
	_h(size_y)
{
	InitializeData();
}

ImageData::~ImageData() {
	delete[] _data;
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

	if (_data != nullptr) {
		delete[] _data;
	}

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

wxBitmap* ImageData::CreateBitmap() {
	int output_size = (sizeof(unsigned char) * _w * _h * 3);
	unsigned char *output_data = (unsigned char*)malloc(output_size);
	long int offset = 0;
	int incr = sizeof(unsigned char);

	auto image = wxImage(wxSize(_w, _h));

	for (int y = 0; y < _h; y++) {
		for (int x = 0; x < _w; x++) {
			Rgb pixel = Pixel(x, y);
			output_data[offset]     = std::clamp((int)(pixel.r * 255.f), 0, 255);
			output_data[offset + 1] = std::clamp((int)(pixel.g * 255.f), 0, 255);
			output_data[offset + 2] = std::clamp((int)(pixel.b * 255.f), 0, 255);

			offset += 3;
		}
	}

	image.SetData(output_data);
	auto bitmap = new wxBitmap(image);
	return bitmap;
}

void ImageData::InitializeData() {
	_data = new Rgb[_w * _h];
	for (int i = 0; i < (_w * _h); i++) {
		_data[i] = Rgb();
	}
}


// ============================================================================
// FUNCTIONS

ImageData* encore::LoadImageEXR(path image_path, int downsample = 1) {
	// Clamp downsample factor
	if (downsample < 1) {
		downsample = 1;
	}

	// EXR loading
	RgbaInputFile file(image_path.filename().string().c_str());

	Box2i dw = file.dataWindow();
	int width = dw.max.x - dw.min.x + 1;
	int height = dw.max.y - dw.min.y + 1;

	Array2D<Rgba> pixels = Array2D<Rgba>(height, width);

	file.setFrameBuffer(&pixels[0][0], 1, width);
	file.readPixels(dw.min.y, dw.max.y);
	
	int output_w = width / downsample;
	int output_h = height / downsample;
	ImageData* output = new ImageData(output_w, output_h);
	for (int y1 = 0, y2 = 0; y1 < height; y1 += downsample, y2++) {
		for (int x1 = 0, x2 = 0; x1 < width; x1 += downsample, x2++) {
			Rgba pixel = pixels[y1][x1]; // EXR pixel
			Rgb &out_px = output->Pixel(x2, y2); // ImageData pixel
			out_px.r = pixel.r;
			out_px.g = pixel.g;
			out_px.b = pixel.b;
		}
	}

	return output;
}