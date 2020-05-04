#pragma once
#include <string>
#include <vector>



namespace airs
{
	class Player
	{
		enum : int32_t
		{

		};

	private:
		void* Device;
		void* Context;

	public:
		static std::vector<std::string> AvailablePlayers();

		Player& operator=(Player&& p) noexcept;
		Player(Player&& p) noexcept;
		Player(const std::string& device);
		Player();
		~Player();

		void MakeCurrent();
		void DoplerFactor(float v);
		void SpeedOfSound(float v);
		//void DistanceModel();

		float GetDoplerFactor() const;
		float GetSpeedOfSound() const;
		//void GetDistanceModel();

		void Suspend() const;
		void Process() const;
		void* GetCurrentContext() const;

		std::string GetVendor() const;
		std::string GetVersion() const;
		std::string GetRenderer() const;
		std::string GetExtensions() const;
	};
}