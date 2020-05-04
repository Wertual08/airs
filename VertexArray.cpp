#include "airs/VertexArray.h"
#include "gl/glew.h"



namespace airs
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &VArrayID);
	}
	VertexArray::VertexArray(VertexArray&& va) noexcept
	{
		std::swap(VArrayID, va.VArrayID);
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &VArrayID);
	}

	void VertexArray::AttribPointer(const Buffer& b, uint32_t ind, int32_t size, AttribType type, bool norm, int32_t stride, const void* ptr)
	{
		glBindVertexArray(VArrayID);
		b.Bind();
		glEnableVertexAttribArray(ind);
		glVertexAttribPointer(ind, size, static_cast<uint32_t>(type), norm, stride, ptr);
		b.Unbind();
		glBindVertexArray(0);
	}
	void VertexArray::AttribIPointer(const Buffer& b, uint32_t ind, int32_t size, AttribType type, int32_t stride, const void* ptr)
	{
		glBindVertexArray(VArrayID);
		b.Bind();
		glEnableVertexAttribArray(ind);
		glVertexAttribIPointer(ind, size, static_cast<uint32_t>(type), stride, ptr);
		b.Unbind();
		glBindVertexArray(0);
	}
	void VertexArray::AttribLPointer(const Buffer& b, uint32_t ind, int32_t size, AttribType type, int32_t stride, const void* ptr)
	{
		glBindVertexArray(VArrayID);
		b.Bind();
		glEnableVertexAttribArray(ind);
		glVertexAttribLPointer(ind, size, static_cast<uint32_t>(type), stride, ptr);
		b.Unbind();
		glBindVertexArray(0);
	}

	void VertexArray::Render(Primitive type, size_t first, size_t count) const
	{
		glBindVertexArray(VArrayID);
		glDrawArrays(static_cast<uint32_t>(type), first, count);
		glBindVertexArray(0);
	}
	void VertexArray::Render(Primitive type, size_t count) const
	{
		glBindVertexArray(VArrayID);
		glDrawArrays(static_cast<uint32_t>(type), 0, count);
		glBindVertexArray(0);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(VArrayID);
	}
	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}