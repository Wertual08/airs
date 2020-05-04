#include "airs/Speaker.h"
#include "al/al.h"



namespace airs
{
	Speaker::Speaker(Speaker&& s)
	{
		std::swap(SourceID, s.SourceID);
	}
	Speaker& Speaker::operator=(Speaker&& s)
	{
		std::swap(SourceID, s.SourceID);
		return *this;
	}
	Speaker::Speaker(const Sound& sound) : Speaker()
	{
		SetSound(sound);
	}
	Speaker::Speaker()
	{
		alGenSources(1, &SourceID);
	}
	Speaker::~Speaker()
	{
		alDeleteSources(1, &SourceID);
	}

	void Speaker::SetPitch(float v)
	{
		alSourcef(SourceID, AL_PITCH, v);
	}
	void Speaker::SetGain(float v)
	{
		alSourcef(SourceID, AL_GAIN, v);
	}
	void Speaker::SetMaxDistance(float v)
	{
		alSourcef(SourceID, AL_MAX_DISTANCE, v);
	}
	void Speaker::SetRolloffFactor(float v)
	{
		alSourcef(SourceID, AL_ROLLOFF_FACTOR, v);
	}
	void Speaker::SetReferenceDistance(float v)
	{
		alSourcef(SourceID, AL_REFERENCE_DISTANCE, v);
	}
	void Speaker::SetMinGain(float v)
	{
		alSourcef(SourceID, AL_MIN_GAIN, v);
	}
	void Speaker::SetMaxGain(float v)
	{
		alSourcef(SourceID, AL_MAX_GAIN, v);
	}
	void Speaker::SetConeOuterGain(float v)
	{
		alSourcef(SourceID, AL_CONE_OUTER_GAIN, v);
	}
	void Speaker::SetConeInnerAngle(float v)
	{
		alSourcef(SourceID, AL_CONE_INNER_ANGLE, v * 0.01745329251f);
	}
	void Speaker::SetConeOuterAngle(float v)
	{
		alSourcef(SourceID, AL_CONE_OUTER_ANGLE, v * 0.01745329251f);
	}
	void Speaker::SetPosition(float x, float y, float z)
	{
		alSource3f(SourceID, AL_POSITION, x, y, z);
	}
	void Speaker::SetPosition(vec3f vec)
	{
		alSourcefv(SourceID, AL_POSITION, vec);
	}
	void Speaker::SetVelocity(float x, float y, float z)
	{
		alSource3f(SourceID, AL_VELOCITY, x, y, z);
	}
	void Speaker::SetVelocity(vec3f vec)
	{
		alSourcefv(SourceID, AL_VELOCITY, vec);
	}
	void Speaker::SetDirection(float x, float y, float z)
	{
		alSource3f(SourceID, AL_DIRECTION, x, y, z);
	}
	void Speaker::SetDirection(vec3f vec)
	{
		alSourcefv(SourceID, AL_DIRECTION, vec);
	}
	void Speaker::SetPosition(float x, float y)
	{
		alSource3f(SourceID, AL_POSITION, x, y, 0.0f);
	}
	void Speaker::SetPosition(vec2f vec)
	{
		alSource3f(SourceID, AL_POSITION, vec.x, vec.y, 0.0f);
	}
	void Speaker::SetVelocity(float x, float y)
	{
		alSource3f(SourceID, AL_VELOCITY, x, y, 0.0f);
	}
	void Speaker::SetVelocity(vec2f vec)
	{
		alSource3f(SourceID, AL_VELOCITY, vec.x, vec.y, 0.0f);
	}
	void Speaker::SetDirection(float x, float y)
	{
		alSource3f(SourceID, AL_DIRECTION, x, y, 0.0f);
	}
	void Speaker::SetDirection(vec2f vec)
	{
		alSource3f(SourceID, AL_DIRECTION, vec.x, vec.y, 0.0f);
	}
	void Speaker::SetPosition(float x)
	{
		alSource3f(SourceID, AL_POSITION, x, 0.0f, 0.0f);
	}
	void Speaker::SetVelocity(float x)
	{
		alSource3f(SourceID, AL_VELOCITY, x, 0.0f, 0.0f);
	}
	void Speaker::SetDirection(float x)
	{
		alSource3f(SourceID, AL_DIRECTION, x, 0.0f, 0.0f);
	}
	void Speaker::SetSourceRelative(bool v)
	{
		alSourcei(SourceID, AL_SOURCE_RELATIVE, v);
	}
	void Speaker::SetLooping(bool v)
	{
		alSourcei(SourceID, AL_LOOPING, v);
	}
	void Speaker::SetSound(const Sound& sound)
	{
		alSourcei(SourceID, AL_BUFFER, sound.BufferID);
	}
	void Speaker::SetState(State v)
	{
		alSourcei(SourceID, AL_SOURCE_STATE, v);
	}
	void Speaker::SetSoundsProcessed(int32_t v)
	{
		alSourcei(SourceID, AL_BUFFERS_PROCESSED, v);
	}
	void Speaker::SetOffsetSeconds(float v)
	{
		alSourcef(SourceID, AL_SEC_OFFSET, v);
	}
	void Speaker::SetOffsetSamples(float v)
	{
		alSourcef(SourceID, AL_SAMPLE_OFFSET, v);
	}
	void Speaker::SetOffsetBytes(float v)
	{
		alSourcef(SourceID, AL_BYTE_OFFSET, v);
	}

	void Speaker::ClearSound()
	{
		alSourcei(SourceID, AL_BUFFER, 0);
	}

	void Speaker::Rewind() const
	{
		alSourceRewind(SourceID);
	}
	void Speaker::Play() const
	{
		alSourcePlay(SourceID);
	}
	void Speaker::Pause() const
	{
		alSourcePause(SourceID);
	}
	void Speaker::Stop() const
	{
		alSourceStop(SourceID);
	}
	
	float Speaker::GetPitch() const
	{
		float result;
		alGetSourcef(SourceID, AL_PITCH, &result);
		return result;
	}
	float Speaker::GetGain() const
	{
		float result;
		alGetSourcef(SourceID, AL_GAIN, &result);
		return result;
	}
	float Speaker::GetMaxDistance() const
	{
		float result;
		alGetSourcef(SourceID, AL_MAX_DISTANCE, &result);
		return result;
	}
	float Speaker::GetRolloffFactor() const
	{
		float result;
		alGetSourcef(SourceID, AL_ROLLOFF_FACTOR, &result);
		return result;
	}
	float Speaker::GetReferenceDistance() const
	{
		float result;
		alGetSourcef(SourceID, AL_REFERENCE_DISTANCE, &result);
		return result;
	}
	float Speaker::GetMinGain() const
	{
		float result;
		alGetSourcef(SourceID, AL_MIN_GAIN, &result);
		return result;
	}
	float Speaker::GetMaxGain() const
	{
		float result;
		alGetSourcef(SourceID, AL_MAX_GAIN, &result);
		return result;
	}
	float Speaker::GetConeOuterGain() const
	{
		float result;
		alGetSourcef(SourceID, AL_CONE_OUTER_GAIN, &result);
		return result;
	}
	float Speaker::GetConeInnerAngle() const
	{
		float result;
		alGetSourcef(SourceID, AL_CONE_INNER_ANGLE, &result);
		return result;
	}
	float Speaker::GetConeOuterAngle() const
	{
		float result;
		alGetSourcef(SourceID, AL_CONE_OUTER_ANGLE, &result);
		return result;
	}
	vec3f Speaker::GetPosition() const
	{
		vec3f result;
		alGetSourcefv(SourceID, AL_POSITION, result);
		return result;
	}
	vec3f Speaker::GetVelocity() const
	{
		vec3f result;
		alGetSourcefv(SourceID, AL_VELOCITY, result);
		return result;
	}
	vec3f Speaker::GetDirection() const
	{
		vec3f result;
		alGetSourcefv(SourceID, AL_DIRECTION, result);
		return result;
	}
	bool Speaker::GetSourceRelative() const
	{
		int32_t result;
		alGetSourcei(SourceID, AL_SOURCE_RELATIVE, &result);
		return result;
	}
	bool Speaker::GetLooping() const
	{
		int32_t result;
		alGetSourcei(SourceID, AL_LOOPING, &result);
		return result;
	}
	Speaker::State Speaker::GetState() const
	{
		int32_t result;
		alGetSourcei(SourceID, AL_SOURCE_STATE, &result);
		return (State)result;
	}
	int32_t Speaker::GetSoundsQueued() const
	{
		int32_t result;
		alGetSourcei(SourceID, AL_BUFFERS_QUEUED, &result);
		return result;
	}
	int32_t Speaker::GetSoundsProcessed() const
	{
		int32_t result;
		alGetSourcei(SourceID, AL_BUFFERS_PROCESSED, &result);
		return result;
	}
	float Speaker::GetOffsetSeconds() const
	{
		float result;
		alGetSourcef(SourceID, AL_SEC_OFFSET, &result);
		return result;
	}
	float Speaker::GetOffsetSamples() const
	{
		float result;
		alGetSourcef(SourceID, AL_SAMPLE_OFFSET, &result);
		return result;
	}
	float Speaker::GetOffsetBytes() const
	{
		float result;
		alGetSourcef(SourceID, AL_BYTE_OFFSET, &result);
		return result;
	}

	void Speaker::QueueSound(const Sound& sound) 
	{
		alSourceQueueBuffers(SourceID, 1, &sound.BufferID);
	}
	void Speaker::UnqueueSounds(size_t n)
	{
		alSourceUnqueueBuffers(SourceID, n, nullptr);
	}
}