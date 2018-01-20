#pragma once
#include "Primitive.h"

struct RayPacket
{
	const static uint width = 16;
	const static uint height = 16;
	const static uint rayCount = width * height;
	const static uint packetCount = width * height / 4;

	union { __m128 t4[packetCount]; float t[rayCount]; };	// ray->t
	union { __m128 u4[packetCount]; float u[rayCount]; };	// ray->u
	union { __m128 v4[packetCount]; float v[rayCount]; };	// ray->v

	__m128 origX4[packetCount];	// ray->orig.x
	__m128 origY4[packetCount];	// ray->orig.y
	__m128 origZ4[packetCount];	// ray->orig.z

	__m128 dirX4[packetCount];	// ray->dir.x
	__m128 dirY4[packetCount];	// ray->dir.y
	__m128 dirZ4[packetCount];	// ray->dir.z

	union { __m128i hit4[packetCount]; Primitive* hit[rayCount]; };	// hit

	union { __m128i sign4[4]; uint sign[12]; };	// sign

	__m128 fX4, fY4, fZ4, fDir4;	// ray frustum
};