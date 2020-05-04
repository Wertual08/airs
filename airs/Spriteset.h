#pragma once
#include <cstdint>
#include <vector>
#include <ostream>
#include "Sprite.h"
#include "Bitmap.h"
#include "Shader.h"
#include "TextureBuffer.h"



namespace airs
{
	class Spriteset : private TextureBuffer, private std::vector<Sprite>
	{
	private:
		Shader SpritesShader;
		int32_t CameraUID, TimeUID;
		size_t Capacity, Cursor;

		void Extend(size_t capacity);

	public:
		Spriteset(size_t capacity);
		Spriteset();
		~Spriteset();

		void Load(const Bitmap& bmp, int32_t count, int32_t delay, float ax, float ay, float an, float sw, float sh);
		void Load(const Bitmap& bmp);

		size_t Size() const;

		void CameraTime(vec4f camera, int32_t time) const;

		using std::vector<Sprite>::operator[];
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}