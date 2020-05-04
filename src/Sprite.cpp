#include "airs/Sprite.h"



namespace airs
{
	Sprite::Sprite(int32_t i, int32_t count, int32_t delay, float iw, float ih, float ax, float ay, float an)
	{
		TextureIndex = i;
		FramesCount = count;
		FrameDelay = delay;
		ImgboxW = iw;
		ImgboxH = ih;
		AxisX = ax;
		AxisY = ay;
		Angle = an;
	}
	Sprite::~Sprite()
	{
	}
	
	Sprite::Vertex Sprite::Adjust(int32_t delay, vec3f pos, float an, uint32_t color) const
	{
		return {
			{ TextureIndex, FramesCount, delay },
			pos,
			{ ImgboxW, ImgboxH },
			{ AxisX, AxisY, Angle + an },
			color
		};
	}
	Sprite::Vertex Sprite::Adjust(int32_t delay, vec3f pos, vec2f size, float an, uint32_t color) const
	{
		return {
			{ TextureIndex, FramesCount, delay },
			pos,
			size,
			{ AxisX, AxisY, Angle + an },
			color
		};
	}
	Sprite::Vertex Sprite::Adjust(vec3f pos, vec2f size, float an, uint32_t color) const
	{
		return {
			{ TextureIndex, FramesCount, FrameDelay },
			pos,
			size,
			{ AxisX, AxisY, Angle + an },
			color
		};
	}
	Sprite::Vertex Sprite::Adjust(vec3f pos, float an, uint32_t color) const
	{
		return {
			{ TextureIndex, FramesCount, FrameDelay },
			pos,
			{ ImgboxW, ImgboxH },
			{ AxisX, AxisY, Angle + an },
			color
		};
	}
}