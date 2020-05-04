#pragma once
#include <cstdint>
#include <algorithm>
#include "Bindable.h"



namespace airs
{
	class Texture : public Bindable
	{
	public:
		enum class Target : uint32_t
		{
			T1D = 0x0DE0,
			T2D = 0x0DE1,
			T3D = 0x806F,
			T1DArray = 0x8C18,
			T2DArray = 0x8C1A,
			Rectangle = 0x84F5,
			CubeMap = 0x8513,
			CubeMapArray = 0x9009,
			Buffer = 0x8C2A,
			T2Dmustisample = 0x9100,
			T2DMultisampleArray = 0x9102
		};

	protected:
		uint32_t TextureTarget, TextureID;

	public:
		static void Active(uint32_t t);

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&& t) noexcept;
		Texture(Target target);
		~Texture();
		
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}

