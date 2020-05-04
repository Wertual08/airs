#pragma once
#include <cstdint>
#include "math.h"
#include "Sound.h"



namespace airs
{
	class Speaker
	{
		enum State : int32_t
		{

		};

	private:
		uint32_t SourceID = 0;

	public:
		Speaker(const Speaker&) = delete;
		Speaker& operator=(const Speaker&) = delete;
		Speaker(Speaker&& s);
		Speaker& operator=(Speaker&& s);
		Speaker(const Sound& sound);
		Speaker();
		~Speaker();

		void SetPitch(float v);
		void SetGain(float v);
		void SetMaxDistance(float v);
		void SetRolloffFactor(float v);
		void SetReferenceDistance(float v);
		void SetMinGain(float v);
		void SetMaxGain(float v);
		void SetConeOuterGain(float v);	
		void SetConeInnerAngle(float v);
		void SetConeOuterAngle(float v);
		void SetPosition(float x, float y, float z);
		void SetPosition(vec3f vec);
		void SetVelocity(float x, float y, float z);
		void SetVelocity(vec3f vec);
		void SetDirection(float x, float y, float z);
		void SetDirection(vec3f vec);
		void SetPosition(float x, float y);
		void SetPosition(vec2f vec);
		void SetVelocity(float x, float y);
		void SetVelocity(vec2f vec);
		void SetDirection(float x, float y);
		void SetDirection(vec2f vec);
		void SetPosition(float x);
		void SetVelocity(float x);
		void SetDirection(float x);
		void SetSourceRelative(bool v);	
		void SetLooping(bool v);	
		void SetSound(const Sound& sound);	
		void SetState(State v);	
		void SetSoundsProcessed(int32_t v);	
		void SetOffsetSeconds(float v);
		void SetOffsetSamples(float v);	
		void SetOffsetBytes(float v);

		void ClearSound();

		void Rewind() const;
		void Play() const;
		void Pause() const;
		void Stop() const;

		float GetPitch() const;
		float GetGain() const;
		float GetMaxDistance() const;
		float GetRolloffFactor() const;
		float GetReferenceDistance() const;
		float GetMinGain() const;
		float GetMaxGain() const;
		float GetConeOuterGain() const;
		float GetConeInnerAngle() const;
		float GetConeOuterAngle() const;
		vec3f GetPosition() const;
		vec3f GetVelocity() const;
		vec3f GetDirection() const;
		bool GetSourceRelative() const;
		bool GetLooping() const;
		State GetState() const;
		int32_t GetSoundsQueued() const;
		int32_t GetSoundsProcessed() const;
		float GetOffsetSeconds() const;
		float GetOffsetSamples() const;
		float GetOffsetBytes() const;

		void QueueSound(const Sound& sound);
		void UnqueueSounds(size_t n);
	};
}