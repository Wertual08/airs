#include "airs/TextureBuffer.h"
#include <algorithm>
#include "gl/glew.h"



namespace airs
{
	size_t TextureBuffer::MaxSize()
	{
		size_t v;
		if (sizeof(size_t) == 8) glGetInteger64v(GL_MAX_TEXTURE_BUFFER_SIZE, (int64_t*)&v);
		else glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, (int32_t*)&v);
		return v;
	}
	TextureBuffer::TextureBuffer(TextureBuffer&& b) noexcept : Buffer(std::move(b)), Texture(std::move(b))
	{
	}
	TextureBuffer::TextureBuffer(size_t size, const void* data, Usage usage, Format format) : 
		Buffer(Buffer::Target::Texture, size, data, usage), Texture(Texture::Target::Buffer)
	{
		Buffer::Bind();
		Texture::Bind(); 
		glTexBuffer(GL_TEXTURE_BUFFER, format, BufferID);
		Texture::Unbind();
		Buffer::Unbind();
	}
	TextureBuffer::TextureBuffer(size_t size, Usage usage, Format format) : Buffer(Buffer::Target::Texture, size, usage), Texture(Texture::Target::Buffer)
	{
		Buffer::Bind();
		Texture::Bind();
		glTexBuffer(GL_TEXTURE_BUFFER, format, BufferID);
		Texture::Unbind();
		Buffer::Unbind();
	}
	TextureBuffer::TextureBuffer(Format format) : Buffer(Buffer::Target::Texture), Texture(Texture::Target::Buffer)
	{
		Buffer::Bind();
		Texture::Bind();
		glTexBuffer(GL_TEXTURE_BUFFER, format, BufferID);
		Texture::Unbind();
		Buffer::Unbind();
	}
	TextureBuffer::~TextureBuffer()
	{
	}

	void TextureBuffer::Bind() const
	{
		Texture::Bind();
	}
	void TextureBuffer::Unbind() const
	{
		Texture::Unbind();
	}
	void TextureBuffer::BufferBind() const
	{
		Buffer::Bind();
	}
	void TextureBuffer::BufferUnbind() const
	{
		Buffer::Unbind();
	}
}