#pragma once

class HDRSky
{
public:
	HDRSky();
	HDRSky(const char* filename);

	const vec4* buffer() const;
	int width() const;
	int height() const;

private:
	vec4* mData;
	int mWidth;
	int mHeight;
};