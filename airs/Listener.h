#pragma once
#include "math.h"



namespace airs
{
	class Listener
	{
	public:
		static void Gain(float v);
		static void Position(float x, float y, float z);
		static void Position(vec3f vec);
		static void Position(float x, float y);
		static void Position(vec2f vec);
		static void Position(float x);
		static void Velocity(float x, float y, float z);
		static void Velocity(vec3f vec);
		static void Velocity(float x, float y);
		static void Velocity(vec2f vec);
		static void Velocity(float x);
		static void Orientation(float atx, float aty, float atz, float upx, float upy, float upz);
		static void Orientation(vec3f at, vec3f up);

		static vec3f GetPosition();
		static vec3f GetVelocity();
		static vec2<vec3f> GetOrientation();
	};
}