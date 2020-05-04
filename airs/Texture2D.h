#pragma once
#include <cstdint>
#include "Bitmap.h"
#include "Texture.h"



namespace airs
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D();
		Texture2D(const Bitmap& bitmap);
		~Texture2D();
	};
}