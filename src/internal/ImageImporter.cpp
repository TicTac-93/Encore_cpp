#include "ImageImporter.h"
#include <Imath/ImathVec.h>

using namespace encore;
using namespace Imath;


// ============================================================================
// FUNCTIONS

wxImage* encore::LoadImageEXR(path image_path, int downsample = 1) {
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
	
	// Conversion to wxImage
	int output_w = width / downsample;
	int output_h = height / downsample;
	int output_size = (sizeof(unsigned char) * output_w * output_h * 3);
	unsigned char *data = (unsigned char*)malloc(output_size);
	long int offset = 0;
	int incr = sizeof(unsigned char);

	auto *image = new wxImage(wxSize(output_w, output_h));

	for (int y = 0; y < height; y += downsample) {
		for (int x = 0; x < width; x += downsample) {
			Rgba pixel = pixels[y][x];
			data[offset]     = std::clamp((int)(pixel.r * 255.f), 0, 255);
			data[offset + 1] = std::clamp((int)(pixel.g * 255.f), 0, 255);
			data[offset + 2] = std::clamp((int)(pixel.b * 255.f), 0, 255);

			offset += 3;
		}
	}

	image->SetData(data);

	return image;

}