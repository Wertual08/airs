#pragma once
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Spriteset.h"
#include "Sprite.h"
#include "math.h"



namespace airs
{
	class SpritesRenderer
	{
	public:
		class Iterator
		{
			friend class SpritesRenderer;
		private:
			const Spriteset& SpritesSet;
			Sprite::Vertex& Data;
			Iterator(const Spriteset& spriteset, Sprite::Vertex& data);

		public:
			void Set(size_t id, vec3f pos, float an = 0.0f, uint32_t color = 0xffffffffu);
			void Set(size_t id, vec3f pos, vec2f size, float an = 0.0f, uint32_t color = 0xffffffffu);
			void Set(size_t id, float x, float y, float z, float an = 0.0f, uint32_t color = 0xffffffffu);
			void Set(size_t id, float x, float y, float z, float w, float h, float an = 0.0f, uint32_t color = 0xffffffffu);
		};

	private:
		const Spriteset& SpritesSet;

		VertexArray SpritesVAO;
		size_t Capacity, Count;
		mutable Buffer SpritesVBO;
		mutable Sprite::Vertex* Data;
		vec4f SpritesCamera;
		int32_t SpritesTime;

		void Extend(size_t capacity);

	public:
		SpritesRenderer(const Spriteset& spriteset);
		~SpritesRenderer();

		void Clear();
		void Reserve(size_t capacity);
		void Resize(size_t size);
		void Push(size_t id, vec3f pos, float an = 0.0f, uint32_t color = 0xffffffffu);
		void Push(size_t id, vec3f pos, vec2f size, float an = 0.0f, uint32_t color = 0xffffffffu);
		void Push(size_t id, float x, float y, float z, float an = 0.0f, uint32_t color = 0xffffffffu);
		void Push(size_t id, float x, float y, float z, float w, float h, float an = 0.0f, uint32_t color = 0xffffffffu);

		void Set(size_t index, size_t id, vec3f pos, float an = 0.0f, uint32_t color = 0xffffffffu);
		void Set(size_t index, size_t id, vec3f pos, vec2f size, float an = 0.0f, uint32_t color = 0xffffffffu);
		void Set(size_t index, size_t id, float x, float y, float z = 0.0f, float an = 0.0, uint32_t color = 0xffffffffu);
		void Set(size_t index, size_t id, float x, float y, float z, float w, float h, float an = 0.0f, uint32_t color = 0xffffffffu);

		Iterator operator[](size_t index);

		void Camera(float x, float y, float w, float h);
		void Camera(vec4f camera);
		void Position(float x, float y);
		void Position(vec2f pos);
		void Resolution(float w, float h);
		void Resolution(vec2f size);
		void Time(int32_t time);
		
		void Render(float dmin, float dmax, float dclear) const;
		void Render() const;
	};
}