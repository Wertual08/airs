#include "airs/SpritesRenderer.h"
#include "gl/glew.h"



namespace airs
{
	SpritesRenderer::Iterator::Iterator(const Spriteset& spriteset, Sprite::Vertex& data) : SpritesSet(spriteset), Data(data)
	{
	}

	void SpritesRenderer::Iterator::Set(size_t id, vec3f pos, float an, uint32_t color)
	{
		Data = SpritesSet[id].Adjust(pos, an, color);
	}
	void SpritesRenderer::Iterator::Set(size_t id, vec3f pos, vec2f size, float an, uint32_t color)
	{
		Data = SpritesSet[id].Adjust(pos, size, an, color);
	}
	void SpritesRenderer::Iterator::Set(size_t id, float x, float y, float z, float an, uint32_t color)
	{
		Data = SpritesSet[id].Adjust({ x, y, z }, an, color);
	}
	void SpritesRenderer::Iterator::Set(size_t id, float x, float y, float z, float w, float h, float an, uint32_t color)
	{
		Data = SpritesSet[id].Adjust({ x, y, z }, { w, h }, an, color);
	}

	void SpritesRenderer::Extend(size_t capacity)
	{
		if (Capacity == 0)
		{
			if (Data)
			{
				SpritesVBO.Unmap();
				Data = nullptr;
			}
			Capacity = capacity;
			SpritesVBO.Data(Capacity * sizeof(Sprite::Vertex), nullptr, Buffer::Usage::DynamicDraw);
		}
		else
		{
			if (Data)
			{
				SpritesVBO.Unmap();
				Data = nullptr;
			}
			while (Capacity < capacity) Capacity *= 2;

			Sprite::Vertex* temp = new Sprite::Vertex[Count];

			SpritesVBO.GetSubData(0, Count * sizeof(Sprite::Vertex), temp);

			SpritesVBO.Data(Capacity * sizeof(Sprite::Vertex), nullptr, Buffer::Usage::DynamicDraw);

			SpritesVBO.SubData(0, Count * sizeof(Sprite::Vertex), temp);

			delete[] temp;
		}
	}

	SpritesRenderer::SpritesRenderer(const Spriteset& spriteset) : 
		SpritesVBO(Buffer::Target::Array), SpritesSet(spriteset),
		SpritesCamera(0.0f, 0.0f, 1.0f, 1.0f), SpritesTime(0)
	{
		Capacity = 0;
		Count = 0;
		Data = nullptr; 
		
		SpritesVAO.AttribIPointer(SpritesVBO, 0, 3, VertexArray::Int, sizeof(Sprite::Vertex), nullptr);
		SpritesVAO.AttribPointer(SpritesVBO, 1, 3, VertexArray::Float, false, sizeof(Sprite::Vertex), (void*)12);
		SpritesVAO.AttribPointer(SpritesVBO, 2, 2, VertexArray::Float, false, sizeof(Sprite::Vertex), (void*)24);
		SpritesVAO.AttribPointer(SpritesVBO, 3, 3, VertexArray::Float, false, sizeof(Sprite::Vertex), (void*)32);
		SpritesVAO.AttribIPointer(SpritesVBO, 4, 1, VertexArray::UInt, sizeof(Sprite::Vertex), (void*)44);
	}
	SpritesRenderer::~SpritesRenderer()
	{
	}

	void SpritesRenderer::Clear()
	{
		Count = 0;
	}
	void SpritesRenderer::Reserve(size_t capacity)
	{
		if (Data)
		{
			SpritesVBO.Unmap();
			Data = nullptr;
		}
		Capacity = capacity;
		SpritesVBO.Data(Capacity * sizeof(Sprite::Vertex), nullptr, Buffer::Usage::DynamicDraw);
		Count = 0;
	}
	void SpritesRenderer::Resize(size_t size)
	{
		if (size > Capacity) Extend(size);
		Count = size;
	}

	void SpritesRenderer::Push(size_t id, vec3f pos, float an, uint32_t color)
	{
		if (Count >= Capacity) Extend(Count + 1);
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + Count++, &SpritesSet[id].Adjust(pos, an, color), sizeof(Sprite::Vertex));
	}
	void SpritesRenderer::Push(size_t id, vec3f pos, vec2f size, float an, uint32_t color)
	{
		if (Count >= Capacity) Extend(Count + 1);
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + Count++, &SpritesSet[id].Adjust(pos, size, an, color), sizeof(Sprite::Vertex));
	}
	void SpritesRenderer::Push(size_t id, float x, float y, float z, float an, uint32_t color)
	{
		if (Count >= Capacity) Extend(Count + 1);
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + Count++, &SpritesSet[id].Adjust({ x, y, z }, an, color), sizeof(Sprite::Vertex));
	}
	void SpritesRenderer::Push(size_t id, float x, float y, float z, float w, float h, float an, uint32_t color)
	{
		if (Count >= Capacity) Extend(Count + 1);
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + Count++, &SpritesSet[id].Adjust({ x, y, z }, { w, h }, an, color), sizeof(Sprite::Vertex));
	}

	void SpritesRenderer::Set(size_t index, size_t id, vec3f pos, float an, uint32_t color)
	{
		if (index >= Count) throw std::out_of_range("airs::SpritesRenderer error: Index was out of range.");
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + index, &SpritesSet[id].Adjust(pos, an, color), sizeof(Sprite::Vertex));
	}
	void SpritesRenderer::Set(size_t index, size_t id, vec3f pos, vec2f size, float an, uint32_t color)
	{
		if (index >= Count) throw std::out_of_range("airs::SpritesRenderer error: Index was out of range.");
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + index, &SpritesSet[id].Adjust(pos, size, an, color), sizeof(Sprite::Vertex));
	}
	void SpritesRenderer::Set(size_t index, size_t id, float x, float y, float z, float an, uint32_t color)
	{
		if (index >= Count) throw std::out_of_range("airs::SpritesRenderer error: Index was out of range.");
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + index, &SpritesSet[id].Adjust({ x, y, z }, an, color), sizeof(Sprite::Vertex));
	}
	void SpritesRenderer::Set(size_t index, size_t id, float x, float y, float z, float w, float h, float an, uint32_t color)
	{
		if (index >= Count) throw std::out_of_range("airs::SpritesRenderer error: Index was out of range.");
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		memcpy(Data + index, &SpritesSet[id].Adjust({ x, y, z }, { w, h }, an, color), sizeof(Sprite::Vertex));
	}

	SpritesRenderer::Iterator SpritesRenderer::operator[](size_t index)
	{
		if (index >= Count) throw std::out_of_range("airs::SpritesRenderer error: Index was out of range.");
		if (!Data) Data = static_cast<Sprite::Vertex*>(SpritesVBO.Map(Buffer::Access::WriteOnly));
		return Iterator(SpritesSet, *(Data + index));
	}

	void SpritesRenderer::Camera(float x, float y, float w, float h)
	{
		SpritesCamera = { x, y, w / 2.0f, h / 2.0f };
	}
	void SpritesRenderer::Camera(vec4f camera)
	{
		SpritesCamera = { camera.x, camera.y, camera.z / 2.0f, camera.w / 2.0f };
	}
	void SpritesRenderer::Position(float x, float y)
	{
		SpritesCamera.x = x;
		SpritesCamera.y = y;
	}
	void SpritesRenderer::Position(vec2f pos)
	{
		SpritesCamera.x = pos.x;
		SpritesCamera.y = pos.y;
	}
	void SpritesRenderer::Resolution(float w, float h)
	{
		SpritesCamera.z = w / 2.0f;
		SpritesCamera.w = h / 2.0f;
	}
	void SpritesRenderer::Resolution(vec2f size)
	{
		SpritesCamera.z = size.x / 2.0f;
		SpritesCamera.w = size.y / 2.0f;
	}
	void SpritesRenderer::Time(int32_t time)
	{
		SpritesTime = time;
	}

	void SpritesRenderer::Render(float dmin, float dmax, float dclear) const
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER | GL_EQUAL);
		glDepthRange(dmin, dmax);
		glClearDepth(dclear);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (Data)
		{
			SpritesVBO.Unmap();
			Data = nullptr;
		}
		Texture::Active(0);
		SpritesSet.CameraTime(SpritesCamera, SpritesTime);
		SpritesSet.Bind();
		SpritesVAO.Render(VertexArray::Points, Count);
		SpritesSet.Unbind();
	}
	void SpritesRenderer::Render() const
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (Data) 
		{
			SpritesVBO.Unmap();
			Data = nullptr;
		}
		Texture::Active(0);
		SpritesSet.CameraTime(SpritesCamera, SpritesTime);
		SpritesSet.Bind();
		SpritesVAO.Render(VertexArray::Points, Count);
		SpritesSet.Unbind();
	}
}