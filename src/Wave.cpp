#include "airs/Wave.h"
#include <fstream>



namespace airs
{
	Wave::Wave(const std::string& file)
	{
		std::ifstream fin(file, std::ios::binary);
		if (!fin) throw std::runtime_error(("airs::Wave error: File [" + file + "] does not exists.").c_str());

		struct WaveHeaderType
		{
			char chunkId[4];
			uint32_t chunkSize;
			char format[4];
			char subChunkId[4];
			uint32_t subChunkSize;
			uint16_t audioFormat;
			uint16_t numChannels;
			int32_t sampleRate;
			uint32_t bytesPerSecond;
			uint16_t blockAlign;
			uint16_t bitsPerSample;
			char dataChunkId[4];
			uint32_t dataSize;
		} WaveHeader;
		enum FileFormat
		{
			WAVE_FORMAT_PCM = 0x0001,
			WAVE_FORMAT_IEEE_FLOAT = 0x0003,
			WAVE_FORMAT_ALAW = 0x0006,
			WAVE_FORMAT_MULAW = 0x0007,
			WAVE_FORMAT_EXTENSIBLE = 0xFFFE
		};

		fin.read((char*)&WaveHeader, sizeof(WaveHeaderType));

		if (WaveHeader.chunkId[0] != 'R' ||
			WaveHeader.chunkId[1] != 'I' ||
			WaveHeader.chunkId[2] != 'F' ||
			WaveHeader.chunkId[3] != 'F' ||
			WaveHeader.format[0] != 'W' ||
			WaveHeader.format[1] != 'A' ||
			WaveHeader.format[2] != 'V' ||
			WaveHeader.format[3] != 'E' ||
			WaveHeader.subChunkId[0] != 'f' ||
			WaveHeader.subChunkId[1] != 'm' ||
			WaveHeader.subChunkId[2] != 't' ||
			WaveHeader.subChunkId[3] != ' ' ||
			//WaveHeader.audioFormat != WAVE_FORMAT_PCM ||
			//WaveHeader.numChannels != 2 ||
			//WaveHeader.sampleRate != 44100 ||
			//WaveHeader.bitsPerSample != 16 ||
			WaveHeader.dataChunkId[0] != 'd' ||
			WaveHeader.dataChunkId[1] != 'a' ||
			WaveHeader.dataChunkId[2] != 't' ||
			WaveHeader.dataChunkId[3] != 'a')
			throw std::runtime_error("airs::Wave error: Invalid wave file header.");

		WaveSize = WaveHeader.dataSize;
		WaveSampleRate = WaveHeader.sampleRate;
		if (WaveHeader.numChannels == 1)
		{
			if (WaveHeader.bitsPerSample == 8) WaveFormat = Mono8;
			else if (WaveHeader.bitsPerSample == 16) WaveFormat = Mono16;
			else WaveFormat = Invalid;
		}
		else if (WaveHeader.numChannels == 2)
		{
			if (WaveHeader.bitsPerSample == 8) WaveFormat = Stereo8;
			else if (WaveHeader.bitsPerSample == 16) WaveFormat = Stereo16;
			else WaveFormat = Invalid;
		}
		else WaveFormat = Invalid;

		WaveData = std::make_unique<uint8_t[]>(WaveSize);
		fin.read((char*)WaveData.get(), WaveSize);
		fin.close();
	}
	Wave::Wave(const char* file) : Wave(std::string(file))
	{
	}
	Wave::Wave(const void* data)
	{
		struct WaveHeaderType
		{
			char chunkId[4];
			uint32_t chunkSize;
			char format[4];
			char subChunkId[4];
			uint32_t subChunkSize;
			uint16_t audioFormat;
			uint16_t numChannels;
			int32_t sampleRate;
			uint32_t bytesPerSecond;
			uint16_t blockAlign;
			uint16_t bitsPerSample;
			char dataChunkId[4];
			uint32_t dataSize;
		} WaveHeader;
		enum FileFormat
		{
			WAVE_FORMAT_PCM = 0x0001,
			WAVE_FORMAT_IEEE_FLOAT = 0x0003,
			WAVE_FORMAT_ALAW = 0x0006,
			WAVE_FORMAT_MULAW = 0x0007,
			WAVE_FORMAT_EXTENSIBLE = 0xFFFE
		};

		memcpy(&WaveHeader, data, sizeof(WaveHeaderType));

		if (WaveHeader.chunkId[0] != 'R' ||
			WaveHeader.chunkId[1] != 'I' ||
			WaveHeader.chunkId[2] != 'F' ||
			WaveHeader.chunkId[3] != 'F' ||
			WaveHeader.format[0] != 'W' ||
			WaveHeader.format[1] != 'A' ||
			WaveHeader.format[2] != 'V' ||
			WaveHeader.format[3] != 'E' ||
			WaveHeader.subChunkId[0] != 'f' ||
			WaveHeader.subChunkId[1] != 'm' ||
			WaveHeader.subChunkId[2] != 't' ||
			WaveHeader.subChunkId[3] != ' ' ||
			//WaveHeader.audioFormat != WAVE_FORMAT_PCM ||
			//WaveHeader.numChannels != 2 ||
			//WaveHeader.sampleRate != 44100 ||
			//WaveHeader.bitsPerSample != 16 ||
			WaveHeader.dataChunkId[0] != 'd' ||
			WaveHeader.dataChunkId[1] != 'a' ||
			WaveHeader.dataChunkId[2] != 't' ||
			WaveHeader.dataChunkId[3] != 'a')
			throw std::runtime_error("airs::Wave error: Invalid wave file header.");

		WaveSize = WaveHeader.dataSize;
		WaveSampleRate = WaveHeader.sampleRate;
		WaveChannels = WaveHeader.numChannels;
		if (WaveChannels == 1)
		{
			if (WaveHeader.bitsPerSample == 8) WaveFormat = Mono8;
			else if (WaveHeader.bitsPerSample == 16) WaveFormat = Mono16;
			else WaveFormat = Invalid;
		}
		else if (WaveChannels == 2)
		{
			if (WaveHeader.bitsPerSample == 8) WaveFormat = Stereo8;
			else if (WaveHeader.bitsPerSample == 16) WaveFormat = Stereo16;
			else WaveFormat = Invalid;
		}
		else WaveFormat = Invalid;

		WaveData = std::make_unique<uint8_t[]>(WaveSize);
		memcpy(WaveData.get(), (char*)data + sizeof(WaveHeaderType), WaveSize);
	}
	Wave::Wave(const Wave& wave)
	{
		WaveSize = wave.WaveSize;
		WaveChannels = wave.WaveChannels;
		WaveSampleRate = wave.WaveSampleRate;
		WaveFormat = wave.WaveFormat;

		WaveData = std::make_unique<uint8_t[]>(WaveSize);
		memcpy(WaveData.get(), wave.WaveData.get(), WaveSize);
	}
	Wave::Wave(Wave&& wave)
	{
		std::swap(WaveSize, wave.WaveSize);
		std::swap(WaveChannels, wave.WaveChannels);
		std::swap(WaveSampleRate, wave.WaveSampleRate);
		std::swap(WaveFormat, wave.WaveFormat);
		std::swap(WaveData, wave.WaveData);
	}
	Wave::~Wave()
	{
	}
	uint32_t Wave::Size() const
	{
		return WaveSize;
	}
	Wave::Format_t Wave::Format() const
	{
		return WaveFormat;
	}
	int32_t Wave::Channels() const
	{
		return WaveChannels;
	}
	int32_t Wave::Frequency() const
	{
		return WaveSampleRate;
	}
	Wave::operator const uint8_t* () const
	{
		return WaveData.get();
	}
}