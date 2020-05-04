#pragma once
#include <cstdint>
#include "Wave.h"



namespace airs
{
	class Sound
	{
		friend class Speaker;
	public:
		enum Format : int32_t
		{
			Mono16 = 0x1101,
			Mono8 = 0x1100,
			Stereo16 = 0x1103,
			Stereo8 = 0x1102,
		};

	private:
		uint32_t BufferID = 0;

	public:
		Sound(const Sound&) = delete;
		Sound& operator=(const Sound&) = delete;
		Sound(Sound&& s) noexcept;
		Sound& operator=(Sound&& s) noexcept;
		Sound(Format format, const void* data, int32_t size, int32_t freq);
		Sound(const Wave& wave);
		Sound(int32_t freq, float seconds);
		Sound();
		~Sound();

		void Data(Format format, const void* data, int32_t size, int32_t freq);
		
		int32_t GetFrequency() const;
		int32_t GetBits() const;
		int32_t GetChannels() const;
		int32_t GetSize() const;

		void SetFrequency(int32_t v);
		void SetBits(int32_t v);
		void SetChannels(int32_t v);
		void SetSize(int32_t v);
	};
}