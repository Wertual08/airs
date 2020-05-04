#include "airs/Player.h"
#include <stdexcept>
#include "al/alc.h"
#include "al/al.h"




namespace airs
{
	std::vector<std::string> Player::AvailablePlayers()
	{
		std::vector<std::string> result;
		const char* device = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
		const char* next = device + 1;
		while (device && *device != '\0' && next && *next != '\0') 
		{
			result.push_back(device);
			size_t len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
		return std::move(result);
	}

	Player& Player::operator=(Player&& p) noexcept
	{
		std::swap(Device, p.Device);
		std::swap(Context, p.Context);
		return *this;
	}
	Player::Player(Player&& p) noexcept
	{
		std::swap(Device, p.Device);
		std::swap(Context, p.Context);
	}
	Player::Player(const std::string& device)
	{
		if (device.empty()) Device = alcOpenDevice(nullptr);
		else Device = alcOpenDevice(device.c_str());
		if (!Device) throw std::runtime_error("airs::Sound error: Device creation failed.");
		Context = alcCreateContext(static_cast<ALCdevice*>(Device), nullptr);
		if (!alcMakeContextCurrent(static_cast<ALCcontext*>(Context))) 
			throw std::runtime_error("airs::Sound error: Make sound context current failed.");
	} 
	Player::Player() : Player("")
	{
	}
	Player::~Player()
	{
		alcDestroyContext(static_cast<ALCcontext*>(Context));
		alcCloseDevice(static_cast<ALCdevice*>(Device));
		Context = nullptr;
		Device = nullptr;
	}
	
	void Player::MakeCurrent()
	{
		alcMakeContextCurrent(static_cast<ALCcontext*>(Context));
	}
	void Player::DoplerFactor(float v)
	{
		alDopplerFactor(v);
	}
	void Player::SpeedOfSound(float v)
	{
		alSpeedOfSound(v);
	}

	float Player::GetDoplerFactor() const
	{
		return alGetFloat(AL_DOPPLER_FACTOR);
	}
	float Player::GetSpeedOfSound() const
	{
		return alGetFloat(AL_SPEED_OF_SOUND);
	}

	void Player::Suspend() const
	{
		alcSuspendContext(static_cast<ALCcontext*>(Context));
	}
	void Player::Process() const
	{
		alcProcessContext(static_cast<ALCcontext*>(Context));
	}
	void* Player::GetCurrentContext() const
	{
		return alcGetCurrentContext();
	}

	std::string Player::GetVendor() const
	{
		return std::move(std::string(alGetString(AL_VENDOR)));
	}
	std::string Player::GetVersion() const
	{
		return std::move(std::string(alGetString(AL_VERSION)));
	}
	std::string Player::GetRenderer() const
	{
		return std::move(std::string(alGetString(AL_RENDERER)));
	}
	std::string Player::GetExtensions() const
	{
		return std::move(std::string(alGetString(AL_EXTENSIONS)));
	}
}