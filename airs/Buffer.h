#pragma once
#include <cstdint>
#include <algorithm>
#include "Bindable.h"



namespace airs
{
	class Buffer : public Bindable
	{
	public:
		enum class Target : uint32_t
		{
			Array = 0x8892,
			AtomicCounter = 0x92C0,
			CopyRead = 0x8F36,
			CopyWrite = 0x8F37,
			DispatchIndirect = 0x90EE,
			DrawIndirect = 0x8F3F,
			ElementArray = 0x8893,
			PixelPack = 0x88EB,
			PixelUnpack = 0x88EC,
			Query = 0x9192,
			ShaderStorage = 0x90D2,
			Texture = 0x8C2A,
			TransformFeedbacd = 0x8C8E,
			Uniform = 0x8A11
		};
		enum class Usage : uint32_t
		{
			StaticDraw = 0x88E4,
			DynamicDraw = 0x88E8,
			StreamDraw = 0x88E0
		};
		enum class Access : uint32_t
		{
			ReadOnly = 0x88B8,
			WriteOnly = 0x88B9,
			ReadWrite = 0x88BA
		};

	protected:
		uint32_t BufferTarget, BufferID;

	public:
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		Buffer(Buffer&& b) noexcept;
		Buffer(Target target, size_t size, const void* data, Usage usage);
		Buffer(Target target, size_t size, Usage usage);
		Buffer(Target target);
		~Buffer();

		void Extend(size_t from, size_t capacity, Usage usage);
		void FastExtend(size_t from, size_t capacity, Usage usage);

		void Data(size_t size, const void* data, Usage usage);
		void SubData(size_t offset, size_t size, const void* data);
		void PixelSubData(size_t offset, size_t size, const void* data);
		void GetSubData(size_t offset, size_t size, void* data) const;
		void* MapRange(size_t offset, size_t size, Access access);
		void* Map(Access access);
		void Unmap();

		void FastData(size_t size, const void* data, Usage usage);
		void FastSubData(size_t offset, size_t size, const void* data);
		void FastGetSubData(size_t offset, size_t size, void* data) const;
		void* FastMapRange(size_t offset, size_t size, Access access);
		void* FastMap(Access access);
		void FastUnmap();

		size_t Size() const;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}

