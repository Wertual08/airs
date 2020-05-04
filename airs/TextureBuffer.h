#pragma once
#include <cstdint>
#include "Buffer.h"
#include "Texture.h"



namespace airs
{
	class TextureBuffer : public Texture, public Buffer
	{
	public:
		enum Format : uint32_t
		{
			R8 = 0x8229,
			R16 = 0x822A,
			R16F = 0x822D,
			R32F = 0x822E,
			R8I = 0x8231,
			R16I = 0x8233,
			R32I = 0x8235,
			R8UI = 0x8232,
			R16UI = 0x8234,
			R32UI = 0x8236,
			RG8 = 0x822B,
			RG16 = 0x822C,
			RG16F = 0x822F,
			RG32F = 0x8230,
			RG8I = 0x8237,
			RG16I = 0x8239,
			RG32I = 0x823B,
			RG8UI = 0x8238,
			RG16UI = 0x823A,
			RG32UI = 0x823C,
			RGB32F = 0x8815,
			RGB32I = 0x8D83,
			RGB32UI = 0x8D71,
			RGBA8 = 0x8058,
			RGBA16 = 0x805B,
			RGBA16F = 0x881A,
			RGBA32F = 0x8814,
			RGBA8I = 0x8D8E,
			RGBA16I = 0x8D88,
			RGBA32I = 0x8D7D,
			RGBA8UI = 0x8D7C,
			RGBA16UI = 0x8D76,
			RGBA32UI = 0x8D70,
		};

	public:
		static size_t MaxSize();

		TextureBuffer(TextureBuffer&& buffer) noexcept;
		TextureBuffer(size_t size, const void* data, Usage usage, Format format);
		TextureBuffer(size_t size, Usage usage, Format format);
		TextureBuffer(Format format);
		~TextureBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		void BufferBind() const;
		void BufferUnbind() const;
	};
}

