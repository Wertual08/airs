#include "airs/Sound.h"
#include <algorithm>
#include "al/al.h"



namespace airs
{
	Sound::Sound(Sound&& s) noexcept
	{
		//alGenBuffers(1, &BufferID);
		std::swap(BufferID, s.BufferID);
	}
	Sound& Sound::operator=(Sound&& s) noexcept
	{
		//alGenBuffers(1, &BufferID);
		std::swap(BufferID, s.BufferID);
		return *this;
	}
	Sound::Sound(Format format, const void* data, int32_t size, int32_t freq) : Sound()
	{
		Data(format, data, size, freq);
	}
	Sound::Sound(const Wave& wave) : Sound()
	{
		Data((Format)wave.Format(), wave, wave.Size(), wave.Frequency());
	}
	Sound::Sound(int32_t freq, float seconds) : Sound()
	{
		uint32_t sample_rate = 10000;
		size_t buf_size = seconds * sample_rate;
		short* samples = new short[buf_size];
		for (uint32_t i = 0; i < buf_size; i++)
			samples[i] = 32760 * sin(2 * 3.14f * i * freq / sample_rate);
		alBufferData(BufferID, AL_FORMAT_MONO16, samples, buf_size, sample_rate);
	}
	Sound::Sound()
	{
		alGenBuffers(1, &BufferID);
	}
	Sound::~Sound()
	{
		alDeleteBuffers(1, &BufferID);
	}
	void Sound::Data(Format format, const void* data, int32_t size, int32_t freq)
	{
		alBufferData(BufferID, format, data, size, freq);
	}

	int32_t Sound::GetFrequency() const
	{
		int32_t result;
		alGetBufferi(BufferID, AL_FREQUENCY, &result);
		return result;
	}
	int32_t Sound::GetBits() const
	{
		int32_t result;
		alGetBufferi(BufferID, AL_BITS, &result);
		return result;
	}
	int32_t Sound::GetChannels() const
	{
		int32_t result;
		alGetBufferi(BufferID, AL_CHANNELS, &result);
		return result;
	}
	int32_t Sound::GetSize() const
	{
		int32_t result;
		alGetBufferi(BufferID, AL_SIZE, &result);
		return result;
	}

	void Sound::SetFrequency(int32_t v)
	{
		alSourcei(BufferID, AL_FREQUENCY, v);
	}
	void Sound::SetBits(int32_t v)
	{
		alBufferi(BufferID, AL_BITS, v);
	}
	void Sound::SetChannels(int32_t v)
	{
		alBufferi(BufferID, AL_CHANNELS, v);
	}
	void Sound::SetSize(int32_t v)
	{
		alBufferi(BufferID, AL_SIZE, v);
	}
}