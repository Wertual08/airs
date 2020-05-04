#include "airs/Listener.h"
#include "al/al.h"



namespace airs
{
	void Listener::Gain(float v)
	{
		alListenerf(AL_GAIN, v);
	}
	void Listener::Position(float x, float y, float z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}
	void Listener::Position(vec3f vec)
	{
		alListenerfv(AL_POSITION, vec);
	}
	void Listener::Position(float x, float y)
	{
		alListener3f(AL_POSITION, x, y, 0.0f);
	}
	void Listener::Position(vec2f vec)
	{
		alListener3f(AL_POSITION, vec.x, vec.y, 0.0f);
	}
	void Listener::Position(float x)
	{
		alListener3f(AL_POSITION, x, 0.0f, 0.0f);
	}
	void Listener::Velocity(float x, float y, float z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
	}
	void Listener::Velocity(vec3f vec)
	{
		alListenerfv(AL_VELOCITY, vec);
	}
	void Listener::Velocity(float x, float y)
	{
		alListener3f(AL_VELOCITY, x, y, 0.0f);
	}
	void Listener::Velocity(vec2f vec)
	{
		alListener3f(AL_VELOCITY, vec.x, vec.y, 0.0f);
	}
	void Listener::Velocity(float x)
	{
		alListener3f(AL_VELOCITY, x, 0.0f, 0.0f);
	}
	void Listener::Orientation(float atx, float aty, float atz, float upx, float upy, float upz)
	{
		float orientation[] = { atx, aty, atz, upx, upy, upz };
		alListenerfv(AL_ORIENTATION, orientation);
	}
	void Listener::Orientation(vec3f at, vec3f up)
	{
		float orientation[] = { at.x, at.y, at.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, orientation);
	}
	vec3f Listener::GetPosition()
	{
		vec3f result;
		alGetListenerfv(AL_POSITION, result);
		return result;
	}
	vec3f Listener::GetVelocity()
	{
		vec3f result;
		alGetListenerfv(AL_VELOCITY, result);
		return result;
	}
	vec2<vec3f> Listener::GetOrientation()
	{
		return vec2<vec3f>();
	}
}