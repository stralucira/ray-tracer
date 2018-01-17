#pragma once

class HDRBitmap
{
public:
	HDRBitmap();
	HDRBitmap(const char* inputfile);

	int width;
	int height;
	vec4* buffer;
};