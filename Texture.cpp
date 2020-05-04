#include "airs/Texture.h"
#include "gl/glew.h"



namespace airs
{
	void Texture::Active(uint32_t t)
	{
		glActiveTexture(GL_TEXTURE0 + t);
	}

	Texture::Texture(Texture&& t) noexcept
	{
		std::swap(TextureTarget, t.TextureTarget);
		std::swap(TextureID, t.TextureID);
	}
	Texture::Texture(Target target) : TextureTarget(static_cast<uint32_t>(target))
	{
		glGenTextures(1, &TextureID);
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &TextureID);
	}

	void Texture::Bind() const
	{
		glBindTexture(TextureTarget, TextureID);
	}
	void Texture::Unbind() const
	{
		glBindTexture(TextureTarget, 0);
	}
}
