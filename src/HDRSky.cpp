#include "template.h"
#include "HDRSky.h"
#include <FreeImage.h>

HDRSky::HDRSky() : mWidth(0), mHeight(0), mData(nullptr) {}

HDRSky::HDRSky(const char* filename)
{
	// device-independent bitmap
	FIBITMAP* dib = FreeImage_Load(FIF_HDR, filename);
	float* buffer = (float*)FreeImage_GetBits(dib);
	mWidth = FreeImage_GetWidth(dib);
	mHeight = FreeImage_GetHeight(dib);

	mData = (vec4*)_aligned_malloc(sizeof(vec4)* mWidth * mHeight, 16);
	for (int y = 0; y < mHeight; y++)
	{
		vec3* line = (vec3*)FreeImage_GetScanLine(dib, mHeight - 1 - y);
		for (int x = 0; x < mWidth; x++)
		{
			mData[y * mWidth + x] = vec4(line[x].x, line[x].y, line[x].z, 1.0f);
		}
	}
}

const vec4* HDRSky::buffer() const
{
	return mData;
}

int HDRSky::width() const
{
	return mWidth;
}

int HDRSky::height() const
{
	return mHeight;
}