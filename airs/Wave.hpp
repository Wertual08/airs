#pragma once
#include <memory>
#include <string>



namespace airs
{
	class Wave
	{
		friend class Speaker;

	public:
		enum Format_t : int32_t
		{
			Invalid = 0,
			Mono16 = 0x1101,
			Mono8 = 0x1100,
			Stereo16 = 0x1103,
			Stereo8 = 0x1102,
		};

	private:
		std::unique_ptr<uint8_t[]> WaveData;
		uint32_t WaveSize;
		Format_t WaveFormat;
		int32_t WaveChannels;
		int32_t WaveSampleRate;

	public:
		Wave(const std::string& file);
		Wave(const char* file);
		Wave(const void* data);
		Wave(const Wave& wave);
		Wave(Wave&& wave);
		~Wave();

		uint32_t Size() const;
		Format_t Format() const;
		int32_t Channels() const;
		int32_t Frequency() const;
		operator const uint8_t* () const;
	};
}