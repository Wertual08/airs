#pragma once
#include <cstdint>
#include "math.h"



namespace airs
{
	class Sprite
	{
	public:
		struct Vertex
		{
			vec3i Metrics;
			vec3f Position;
			vec2f Size;
			vec3f Transform;
			uint32_t Color;
		};

		int32_t TextureIndex;
		int32_t FramesCount;
		int32_t FrameDelay;
		float ImgboxW;
		float ImgboxH;
		float AxisX;
		float AxisY;
		float Angle;

		Sprite(int32_t i, int32_t count, int32_t delay, float iw, float ih, float ax, float ay, float an);
		~Sprite();

		Vertex Adjust(int32_t delay, vec3f pos, float an, uint32_t color) const;
		Vertex Adjust(int32_t delay, vec3f pos, vec2f size, float an, uint32_t color) const;
		Vertex Adjust(vec3f pos, vec2f size, float an, uint32_t color) const;
		Vertex Adjust(vec3f pos, float an, uint32_t color) const;
	};
}