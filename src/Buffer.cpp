#include "airs/Buffer.h"
#include "gl/glew.h"



namespace airs
{
	Buffer::Buffer(Buffer&& b) noexcept
	{
		std::swap(BufferTarget, b.BufferTarget);
		std::swap(BufferID, b.BufferID);
	}
	Buffer::Buffer(Target target, size_t size, const void* data, Usage usage) : Buffer(target)
	{
		glBindBuffer(BufferTarget, BufferID);
		glBufferData(BufferTarget, size, nullptr, static_cast<uint32_t>(usage));
		glBindBuffer(BufferTarget, 0);
	}
	Buffer::Buffer(Target target, size_t size, Usage usage) : Buffer(target)
	{
		glBindBuffer(BufferTarget, BufferID);
		glBufferData(BufferTarget, size, nullptr, static_cast<uint32_t>(usage));
		glBindBuffer(BufferTarget, 0);
	}
	Buffer::Buffer(Target target) : BufferTarget(static_cast<uint32_t>(target))
	{
		glGenBuffers(1, &BufferID);
	}
	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &BufferID);
	}

	void Buffer::Extend(size_t from, size_t capacity, Usage usage)
	{
		uint8_t* temp = new uint8_t[from];

		Bind();
		FastGetSubData(0, from, temp);
		FastData(capacity, nullptr, usage);
		FastSubData(0, from, temp);
		Unbind();

		delete[] temp;
	}
	void Buffer::FastExtend(size_t from, size_t capacity, Usage usage)
	{
		uint8_t* temp = new uint8_t[from];

		FastGetSubData(0, from, temp);
		FastData(capacity, nullptr, usage);
		FastSubData(0, from, temp);

		delete[] temp;
	}

	void Buffer::Data(size_t size, const void* data, Usage usage)
	{
		glBindBuffer(BufferTarget, BufferID);
		glBufferData(BufferTarget, size, data, static_cast<uint32_t>(usage));
		glBindBuffer(BufferTarget, 0);
	}
	void Buffer::SubData(size_t offset, size_t size, const void* data)
	{
		glBindBuffer(BufferTarget, BufferID);
		glBufferSubData(BufferTarget, offset, size, data);
		glBindBuffer(BufferTarget, 0);
	}
	void Buffer::PixelSubData(size_t offset, size_t size, const void* data)
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, BufferID);
		glBufferSubData(GL_PIXEL_UNPACK_BUFFER, offset, size, data);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
	void Buffer::GetSubData(size_t offset, size_t size, void* data) const
	{
		glBindBuffer(BufferTarget, BufferID);
		glGetBufferSubData(BufferTarget, offset, size, data);
		glBindBuffer(BufferTarget, 0);
	}
	void* Buffer::MapRange(size_t offset, size_t size, Access access)
	{
		glBindBuffer(BufferTarget, BufferID);
		void* t = nullptr;
		switch (access)
		{
		case Access::ReadOnly: t = glMapBufferRange(BufferTarget, offset, size, GL_MAP_READ_BIT);
		case Access::WriteOnly: t = glMapBufferRange(BufferTarget, offset, size, GL_MAP_WRITE_BIT);
		case Access::ReadWrite: t = glMapBufferRange(BufferTarget, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
		}
		glBindBuffer(BufferTarget, 0);
		return t;
	}
	void* Buffer::Map(Access access)
	{
		glBindBuffer(BufferTarget, BufferID);
		void* t = glMapBuffer(BufferTarget, static_cast<uint32_t>(access));
		glBindBuffer(BufferTarget, 0);
		return t;
	}
	void Buffer::Unmap()
	{
		glBindBuffer(BufferTarget, BufferID);
		glUnmapBuffer(BufferTarget);
		glBindBuffer(BufferTarget, 0);
	}

	void Buffer::FastData(size_t size, const void* data, Usage usage)
	{
		glBufferData(BufferTarget, size, data, static_cast<uint32_t>(usage));
	}
	void Buffer::FastSubData(size_t offset, size_t size, const void* data)
	{
		glBufferSubData(BufferTarget, offset, size, data);
	}
	void Buffer::FastGetSubData(size_t offset, size_t size, void* data) const
	{
		glGetBufferSubData(BufferTarget, offset, size, data);
	}
	void* Buffer::FastMapRange(size_t offset, size_t size, Access access)
	{
		switch (access)
		{
		case Access::ReadOnly: return glMapBufferRange(BufferTarget, offset, size, GL_MAP_READ_BIT);
		case Access::WriteOnly: return glMapBufferRange(BufferTarget, offset, size, GL_MAP_WRITE_BIT);
		case Access::ReadWrite: return glMapBufferRange(BufferTarget, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
		default: return nullptr;
		}
	}
	void* Buffer::FastMap(Access access)
	{
		return glMapBuffer(BufferTarget, static_cast<uint32_t>(access));
	}
	void Buffer::FastUnmap()
	{
		glUnmapBuffer(BufferTarget);
	}

	size_t Buffer::Size() const
	{
		size_t size;
		glBindBuffer(BufferTarget, BufferID);
		glGetBufferParameteri64v(BufferTarget, GL_BUFFER_SIZE, (int64_t*)&size);
		glBindBuffer(BufferTarget, 0);
		return size;
	}
	void Buffer::Bind() const
	{
		glBindBuffer(BufferTarget, BufferID);
	}
	void Buffer::Unbind() const
	{
		glBindBuffer(BufferTarget, 0);
	}
}
