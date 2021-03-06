// Template, major revision 6, update for INFOMOV
// IGAD/NHTV/UU - Jacco Bikker - 2006-2016
// Credits to Mathijs Molenaar for making it both Windows, Linux, and Mac compatible

#pragma once
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#define BLACK				vec3(0.0f)
#define GRAY				vec3(0.5f)
#define WHITE				vec3(1.0f)
#define RED					vec3(1.0f, 0.0f, 0.0f)
#define GREEN				vec3(0.0f, 1.0f, 0.0f)
#define BLUE				vec3(0.0f, 0.0f, 1.0f)
#define BACKGROUND_COLOR	vec3(0.0f, 0.3f, 0.6f)

#ifndef _WIN32
typedef unsigned int uint;	// Already defined in windows environments?
#endif

typedef unsigned char uchar;
typedef unsigned char byte;

#ifdef _WIN32
#include "SDL.h"
#include "SDL_syswm.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#endif

#include "math.h"
#include "stdlib.h"
//#include "emmintrin.h"
#include "stdio.h"
#ifdef _WIN32
#include "windows.h"// Required by wglext.h
#endif

#include "pixel.h"
#include "surface.h"
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <sstream>
#include <omp.h>

#ifdef __linux__
#include <FreeImage.h>
#else
#include <FreeImage.h>
#endif

#ifdef _WIN32
extern "C"
{
#include "glew.h"
}
#include "gl.h"
#include "io.h"
#include "wglext.h"
#else

#include <GL/glew.h>
#ifdef __linux__
#include <GL/gl.h>
#else//  OS X
#include <OpenGL/gl.h>
#endif

#endif

#include <ios>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "fcntl.h"
#include <memory>
#include "emmintrin.h"

//#include <immintrin.h>

#include "game.h"

using namespace Tmpl8;				// to use template classes
using namespace glm;
using namespace std;				// to use stl vectors

inline float Rand(float range) { return ((float)rand() / RAND_MAX) * range; }
inline int IRand(int range) { return rand() % range; }
int filesize( FILE* f );
void delay();

namespace Tmpl8 {

#define PI					3.14159265358979323846264338327950f
#define INVPI				0.31830988618379067153776752674503f
#define PIOVER180			0.01745329251994329576923690768488f

#ifdef WIN32
#define MALLOC64(x)			_aligned_malloc(x,64)
#define FREE64(x)			_aligned_free(x)
#else
inline void* MALLOC64(size_t size)
{
	void* ptr;
	if (posix_memalign(&ptr, 64, size) != 0)
	{
		std::cout << "Aligned memory alloc failed!" << std::endl;
		exit(1);
	}
	return ptr;
}
//#define MALLOC64(x)			posix_memalign(64, x)
#define FREE64(x)			free(x)
#endif
/*#define MALLOC64(x)			_aligned_malloc(x,64)
#define FREE64(x)			_aligned_free(x)
#define PREFETCH(x)			_mm_prefetch((const char*)(x),_MM_HINT_T0)
#define PREFETCH_ONCE(x)	_mm_prefetch((const char*)(x),_MM_HINT_NTA)
#define PREFETCH_WRITE(x)	_m_prefetchw((const char*)(x))
#define loadss(mem)			_mm_load_ss((const float*const)(mem))
#define broadcastps(ps)		_mm_shuffle_ps((ps),(ps), 0)
#define broadcastss(ss)		broadcastps(loadss((ss)))*/
#define MIN(x, y)           (x < y ? x : y)
#define MAX(x, y)           (x > y ? x : y)

struct Timer
{
	typedef std::chrono::high_resolution_clock Clock;
#ifdef __linux__
	typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
#elif _WIN32
	typedef std::chrono::steady_clock::time_point TimePoint;
#else// Both OS X and Windows
	typedef std::chrono::time_point<std::chrono::_V2::system_clock> TimePoint;
#endif
	typedef std::chrono::microseconds MicroSeconds;

	TimePoint start;
	Timer() : start( get() ) { init(); }
	float elapsed() const {
		auto diff = get() - start;
		auto durationMs = std::chrono::duration_cast<MicroSeconds>(
			diff);
		return static_cast<float>(durationMs.count()) / 1000000.0f;
	}
	static TimePoint get()
	{
		return Clock::now();
	}

	//static double to_time(const TimePoint vt) {
	//	// TODO...
	//}

	void reset() { start = get(); }
	static void init()
	{
	}
};

#define BADFLOAT(x) ((*(uint*)&x & 0x7f000000) == 0x7f000000)

// vectors
class float2 // adapted from https://github.com/dcow/RayTracer
{
public:
	union { struct { float x, y; }; float cell[2]; };
	float2() {}
	float2( float v ) : x( v ), y( v ) {}
	float2( float x, float y ) : x( x ), y( y ) {}
	float2 operator - () const { return float2( -x, -y ); }
	float2 operator + ( const float2& addOperand ) const { return float2( x + addOperand.x, y + addOperand.y ); }
	float2 operator - ( const float2& operand ) const { return float2( x - operand.x, y - operand.y ); }
	float2 operator * ( const float2& operand ) const { return float2( x * operand.x, y * operand.y ); }
	float2 operator * ( float operand ) const { return float2( x * operand, y * operand ); }
	void operator -= ( const float2& a ) { x -= a.x; y -= a.y; }
	void operator += ( const float2& a ) { x += a.x; y += a.y; }
	void operator *= ( const float2& a ) { x *= a.x; y *= a.y; }
	void operator *= ( float a ) { x *= a; y *= a; }
	float& operator [] ( const int idx ) { return cell[idx]; }
	float length() { return sqrtf( x * x + y * y ); }
	float sqrLength() { return x * x + y * y; }
	float2 normalized() { float r = 1.0f / length(); return float2( x * r, y * r ); }
	void normalize() { float r = 1.0f / length(); x *= r; y *= r; }
	static float2 normalize( float2 v ) { return v.normalized(); }
	float dot( const float2& operand ) const { return x * operand.x + y * operand.y; }
};

class float3
{
public:
	union { struct { float x, y, z; }; float cell[3]; };
	float3() {}
	float3( float v ) : x( v ), y( v ), z( v ) {}
	float3( float x, float y, float z ) : x( x ), y( y ), z( z ) {}
	float3 operator - () const { return float3( -x, -y, -z ); }
	float3 operator + ( const float3& addOperand ) const { return float3( x + addOperand.x, y + addOperand.y, z + addOperand.z ); }
	float3 operator - ( const float3& operand ) const { return float3( x - operand.x, y - operand.y, z - operand.z ); }
	float3 operator * ( const float3& operand ) const { return float3( x * operand.x, y * operand.y, z * operand.z ); }
	void operator -= ( const float3& a ) { x -= a.x; y -= a.y; z -= a.z; }
	void operator += ( const float3& a ) { x += a.x; y += a.y; z += a.z; }
	void operator *= ( const float3& a ) { x *= a.x; y *= a.y; z *= a.z; }
	void operator *= ( const float a ) { x *= a; y *= a; z *= a; }
	float operator [] ( const uint& idx ) const { return cell[idx]; }
	float& operator [] ( const uint& idx ) { return cell[idx]; }
	float length() const { return sqrtf( x * x + y * y + z * z ); }
	float sqrLength() const { return x * x + y * y + z * z; }
	float3 normalized() const { float r = 1.0f / length(); return float3( x * r, y * r, z * r ); }
	void normalize() { float r = 1.0f / length(); x *= r; y *= r; z *= r; }
	static float3 normalize( const float3 v ) { return v.normalized(); }
	float3 cross( const float3& operand ) const
	{
		return float3( y * operand.z - z * operand.y, z * operand.x - x * operand.z, x * operand.y - y * operand.x );
	}
	float dot( const float3& operand ) const { return x * operand.x + y * operand.y + z * operand.z; }
};

float3 normalize( const float3& v );
float2 normalize( const float2& v );
float3 cross( const float3& a, const float3& b );
float dot( const float3& a, const float3& b );
float dot( const float2& a, const float2& b );
float3 operator * ( const float& s, const float3& v );
float2 operator * ( const float& s, const float2& v );
float3 operator * ( const float3& v, const float& s );
float2 operator * ( const float2& v, const float& s );
float length( const float2& v );

class float4
{
public:
	union { struct { float x, y, z, w; }; float cell[4]; };
	float4() {}
	float4( float v ) : x( v ), y( v ), z( v ), w( v ) {}
	float4( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) {}
	float4( float3 a, float b ) : x( a.x ), y( a.y ), z( a.z ), w( b ) {}
	float4 operator - () const { return float4( -x, -y, -z, -w ); }
	float4 operator + ( const float4& addOperand ) const { return float4( x + addOperand.x, y + addOperand.y, z + addOperand.z, w + addOperand.w ); }
	float4 operator - ( const float4& operand ) const { return float4( x - operand.x, y - operand.y, z - operand.z, w - operand.w ); }
	float4 operator * ( const float4& operand ) const { return float4( x * operand.x, y * operand.y, z * operand.z, w * operand.w ); }
	float4 operator * ( float operand ) const { return float4( x * operand, y * operand, z * operand, w * operand ); }
	void operator -= ( const float4& a ) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
	void operator += ( const float4& a ) { x += a.x; y += a.y; z += a.z; w += a.w; }
	void operator *= ( const float4& a ) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
	void operator *= ( float a ) { x *= a; y *= a; z *= a; w *= a; }
	float& operator [] ( const int idx ) { return cell[idx]; }
	float operator [] ( const uint& idx ) const { return cell[idx]; }
	float length() { return sqrtf( x * x + y * y + z * z + w * w ); }
	float sqrLentgh() { return x * x + y * y + z * z + w * w; }
	float4 normalized() { float r = 1.0f / length(); return float4( x * r, y * r, z * r, w * r ); }
	void normalize() { float r = 1.0f / length(); x *= r; y *= r; z *= r; w *= r; }
	static float4 normalize( float4 v ) { return v.normalized(); }
	float dot( const float4& operand ) const { return x * operand.x + y * operand.y + z * operand.z + w * operand.w; }
};

class uint4
{
public:
	union { struct { uint x, y, z, w; }; uint cell[4]; };
	uint4() {}
	uint4( int v ) : x( v ), y( v ), z( v ), w( v ) {}
	uint4( int x, int y, int z, int w ) : x( x ), y( y ), z( z ), w( w ) {}
	uint4 operator + ( const uint4& addOperand ) const { return uint4( x + addOperand.x, y + addOperand.y, z + addOperand.z, w + addOperand.w ); }
	uint4 operator - ( const uint4& operand ) const { return uint4( x - operand.x, y - operand.y, z - operand.z, w - operand.w ); }
	uint4 operator * ( const uint4& operand ) const { return uint4( x * operand.x, y * operand.y, z * operand.z, w * operand.w ); }
	uint4 operator * ( uint operand ) const { return uint4( x * operand, y * operand, z * operand, w * operand ); }
	void operator -= ( const uint4& a ) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
	void operator += ( const uint4& a ) { x += a.x; y += a.y; z += a.z; w += a.w; }
	void operator *= ( const uint4& a ) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
	void operator *= ( uint a ) { x *= a; y *= a; z *= a; w *= a; }
	uint& operator [] ( const int idx ) { return cell[idx]; }
};

class int4
{
public:
	union { struct { int x, y, z, w; }; int cell[4]; };
	int4() {}
	int4( int v ) : x( v ), y( v ), z( v ), w( v ) {}
	int4( int x, int y, int z, int w ) : x( x ), y( y ), z( z ), w( w ) {}
	int4 operator - () const { return int4( -x, -y, -z, -w ); }
	int4 operator + ( const int4& addOperand ) const { return int4( x + addOperand.x, y + addOperand.y, z + addOperand.z, w + addOperand.w ); }
	int4 operator - ( const int4& operand ) const { return int4( x - operand.x, y - operand.y, z - operand.z, w - operand.w ); }
	int4 operator * ( const int4& operand ) const { return int4( x * operand.x, y * operand.y, z * operand.z, w * operand.w ); }
	int4 operator * ( int operand ) const { return int4( x * operand, y * operand, z * operand, w * operand ); }
	void operator -= ( const int4& a ) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
	void operator += ( const int4& a ) { x += a.x; y += a.y; z += a.z; w += a.w; }
	void operator *= ( const int4& a ) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
	void operator *= ( int a ) { x *= a; y *= a; z *= a; w *= a; }
	int& operator [] ( const int idx ) { return cell[idx]; }
};

class mat4
{
public:
	float cell[16];
	float& operator [] ( const int idx ) { return cell[idx]; }
	static mat4 Identity() { mat4 r; memset( r.cell, 0, 64 ); r.cell[0] = r.cell[5] = r.cell[10] = r.cell[15] = 1.0f; return r; }
	static mat4 RotateX( float a ) { mat4 r = mat4::Identity(); r.cell[5] = cosf( a ); r.cell[6] = -sinf( a ); r.cell[9] = sinf( a ); r.cell[10] = cosf( a ); return r; };
	static mat4 RotateY( float a ) { mat4 r = mat4::Identity(); r.cell[0] = cosf( a ); r.cell[2] = -sinf( a ); r.cell[8] = sinf( a ); r.cell[10] = cosf( a ); return r; };
	static mat4 RotateZ( float a ) { mat4 r = mat4::Identity(); r.cell[0] = cosf( a ); r.cell[1] = -sinf( a ); r.cell[4] = sinf( a ); r.cell[5] = cosf( a ); return r; };
	static mat4 Translate( float x, float y, float z ) { mat4 r = mat4::Identity(); r.cell[3] = x; r.cell[7] = y; r.cell[11] = z; return r; };
	void Invert()
	{
		// from MESA, via http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		const float inv[16] = {
			 cell[5]  * cell[10] * cell[15] - cell[5]  * cell[11] * cell[14] - cell[9]  * cell[6]  * cell[15] +
			 cell[9]  * cell[7]  * cell[14] + cell[13] * cell[6]  * cell[11] - cell[13] * cell[7]  * cell[10],
			-cell[1]  * cell[10] * cell[15] + cell[1]  * cell[11] * cell[14] + cell[9]  * cell[2]  * cell[15] -
			 cell[9]  * cell[3]  * cell[14] - cell[13] * cell[2]  * cell[11] + cell[13] * cell[3]  * cell[10],
			 cell[1]  * cell[6]  * cell[15] - cell[1]  * cell[7]  * cell[14] - cell[5]  * cell[2]  * cell[15] +
			 cell[5]  * cell[3]  * cell[14] + cell[13] * cell[2]  * cell[7]  - cell[13] * cell[3]  * cell[6],
			-cell[1]  * cell[6]  * cell[11] + cell[1]  * cell[7]  * cell[10] + cell[5]  * cell[2]  * cell[11] -
			 cell[5]  * cell[3]  * cell[10] - cell[9]  * cell[2]  * cell[7]  + cell[9]  * cell[3]  * cell[6],
			-cell[4]  * cell[10] * cell[15] + cell[4]  * cell[11] * cell[14] + cell[8]  * cell[6]  * cell[15] -
			 cell[8]  * cell[7]  * cell[14] - cell[12] * cell[6]  * cell[11] + cell[12] * cell[7]  * cell[10],
			 cell[0]  * cell[10] * cell[15] - cell[0]  * cell[11] * cell[14] - cell[8]  * cell[2]  * cell[15] +
			 cell[8]  * cell[3]  * cell[14] + cell[12] * cell[2]  * cell[11] - cell[12] * cell[3]  * cell[10],
			-cell[0]  * cell[6]  * cell[15] + cell[0]  * cell[7]  * cell[14] + cell[4]  * cell[2]  * cell[15] -
			 cell[4]  * cell[3]  * cell[14] - cell[12] * cell[2]  * cell[7]  + cell[12] * cell[3]  * cell[6],
			 cell[0]  * cell[6]  * cell[11] - cell[0]  * cell[7]  * cell[10] - cell[4]  * cell[2]  * cell[11] +
			 cell[4]  * cell[3]  * cell[10] + cell[8]  * cell[2]  * cell[7]  - cell[8]  * cell[3]  * cell[6],
			 cell[4]  * cell[9]  * cell[15] - cell[4]  * cell[11] * cell[13] - cell[8]  * cell[5]  * cell[15] +
			 cell[8]  * cell[7]  * cell[13] + cell[12] * cell[5]  * cell[11] - cell[12] * cell[7]  * cell[9],
			-cell[0]  * cell[9]  * cell[15] + cell[0]  * cell[11] * cell[13] + cell[8]  * cell[1]  * cell[15] -
			 cell[8]  * cell[3]  * cell[13] - cell[12] * cell[1]  * cell[11] + cell[12] * cell[3]  * cell[9],
			 cell[0]  * cell[5]  * cell[15] - cell[0]  * cell[7]  * cell[13] - cell[4]  * cell[1]  * cell[15] +
			 cell[4]  * cell[3]  * cell[13] + cell[12] * cell[1]  * cell[7]  - cell[12] * cell[3]  * cell[5],
			-cell[0]  * cell[5]  * cell[11] + cell[0]  * cell[7]  * cell[9]  + cell[4]  * cell[1]  * cell[11] -
			 cell[4]  * cell[3]  * cell[9]  - cell[8]  * cell[1]  * cell[7]  + cell[8]  * cell[3]  * cell[5],
			-cell[4]  * cell[9]  * cell[14] + cell[4]  * cell[10] * cell[13] + cell[8]  * cell[5]  * cell[14] -
			 cell[8]  * cell[6]  * cell[13] - cell[12] * cell[5]  * cell[10] + cell[12] * cell[6]  * cell[9],
			 cell[0]  * cell[9]  * cell[14] - cell[0]  * cell[10] * cell[13] - cell[8]  * cell[1]  * cell[14] +
			 cell[8]  * cell[2]  * cell[13] + cell[12] * cell[1]  * cell[10] - cell[12] * cell[2]  * cell[9],
			-cell[0]  * cell[5]  * cell[14] + cell[0]  * cell[6]  * cell[13] + cell[4]  * cell[1]  * cell[14] -
			 cell[4]  * cell[2]  * cell[13] - cell[12] * cell[1]  * cell[6]  + cell[12] * cell[2]  * cell[5],
			 cell[0]  * cell[5]  * cell[10] - cell[0]  * cell[6]  * cell[9]  - cell[4]  * cell[1]  * cell[10] +
			 cell[4]  * cell[2]  * cell[9]  + cell[8]  * cell[1]  * cell[6]  - cell[8]  * cell[2]  * cell[5]
		};
		const float det = cell[0] * inv[0] + cell[1] * inv[4] + cell[2] * inv[8] + cell[3] * inv[12];
		if (det != 0)
		{
			const float invdet = 1.0f / det;
			for( int i = 0; i < 16; i++ ) cell[i] = inv[i] * invdet;
		}
	}
};

mat4 operator * ( const mat4& a, const mat4& b );

inline const vec3 normalizeSSE(vec3 vector)
{
	__m128 tmp;

	// copy data into the 128bit register
	tmp = _mm_set_ps(1.0f, vector.z, vector.y, vector.x);

	// 0x7F = 0111 1111 ~ means we don't want the w-component multiplied
	// and the result written to all 4 components
	__m128 dp = _mm_dp_ps(tmp, tmp, 0x7F);

	// compute rsqrt of the dot product
	dp = _mm_rsqrt_ps(dp);

	// vec * rsqrt(dot(vec, vec))
	tmp = _mm_mul_ps(tmp, dp);

	vec3 vec;
	union { __m128 v; float f[4]; } uf; // to access the 4 floats
	uf.v = tmp;

	vec.x = uf.f[0];
	vec.y = uf.f[1];
	vec.z = uf.f[2];

	return vec;
}

// User added methods
inline const int returnLargest(vec3 point)
{
	return point.x > point.y ?
		(point.x > point.z ? 0 : 2) :
		(point.y > point.z ? 1 : 2);
}

#include "AABB.h"
inline float CalculateDistance2(AABB bounds, vec3 point)
{
	float distanceX = glm::max(glm::max(bounds.min.x - point.x, 0.0f), glm::max(0.0f, point.x - bounds.max.x));
	float distanceY = glm::max(glm::max(bounds.min.y - point.y, 0.0f), glm::max(0.0f, point.y - bounds.max.y));
	float distanceZ = glm::max(glm::max(bounds.min.z - point.z, 0.0f), glm::max(0.0f, point.z - bounds.max.z));

	return distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;
}

}; // namespace Tmpl8