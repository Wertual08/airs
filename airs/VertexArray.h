#pragma once
#include <cstdint>
#include "Bindable.h"
#include "Buffer.h"



namespace airs
{
	class VertexArray : public Bindable
	{
	public:
		enum AttribType : uint32_t
		{
			SByte = 0x1400,
			Byte = 0x1401,
			Short = 0x1402,
			UShort = 0x1403,
			Int = 0x1404,
			UInt = 0x1405,
			Float = 0x1406,
			Double = 0x140A,
			HFloat = 0x140B,
			Fixed = 0x140C,
			//GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV Maybe sometimes...
		};
		enum Primitive : uint32_t
		{
			Points = 0x0000,
			Lines = 0x0001,
			LineLoop = 0x0002,
			LineStrip = 0x0003,
			Triangles = 0x0004,
			TriangleStrip = 0x0005,
			TriangleFan = 0x0006,
			Quads = 0x0007,
			QuadStrip = 0x0008,
			Polygon = 0x0009,
		};

	private:
		uint32_t VArrayID;

	public:
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray();
		VertexArray(VertexArray&& va) noexcept;
		~VertexArray();

		void AttribPointer(const Buffer& b, uint32_t ind, int32_t size, AttribType type, bool norm, int32_t stride, const void* ptr);
		void AttribIPointer(const Buffer& b, uint32_t ind, int32_t size, AttribType type, int32_t stride, const void* ptr);
		void AttribLPointer(const Buffer& b, uint32_t ind, int32_t size, AttribType type, int32_t stride, const void* ptr);

		void Render(Primitive type, size_t first, size_t count) const;
		void Render(Primitive type, size_t count) const;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}

