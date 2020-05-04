#include "airs/Window.h"
#include <cstdio>
#include "airs/Texture2D.h"
#include "gl/glew.h"



namespace airs
{
	Texture2D::Texture2D() : Texture(Target::T2D)
	{
	}
	Texture2D::Texture2D(const Bitmap& bitmap) : Texture2D()
	{
		Texture::Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.Width(), bitmap.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
		Texture::Unbind();
	}
	Texture2D::~Texture2D()
	{
	}
}
