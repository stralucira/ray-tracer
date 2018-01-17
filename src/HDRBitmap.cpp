#include "template.h"
#include "HDRBitmap.h"
#include <FreeImage.h>

HDRBitmap::HDRBitmap() : width(0), height(0), buffer(nullptr) {}

HDRBitmap::HDRBitmap(const char* inputfile)
{
	// device-independent bitmap
	FIBITMAP* dib = FreeImage_Load(FIF_HDR, inputfile);

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	
	buffer = (vec4*)_aligned_malloc(sizeof(vec4)* width * height, 16);
	for (int y = 0; y < height; y++)
	{
		vec3* line = (vec3*)FreeImage_GetScanLine(dib, height - 1 - y);
		for (int x = 0; x < width; x++)
		{
			buffer[y * width + x] = vec4(line[x].x, line[x].y, line[x].z, 1.0f);
		}
	}
}