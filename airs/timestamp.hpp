#pragma once
#include <cstdint>
#include <sstream>
#include <string>



namespace airs
{
	class timestamp
	{
	private:
		std::int64_t FullSeconds;
		double FractionOfSecond;

		inline void AdjustFraction()
		{
			std::int64_t reminder = static_cast<int64_t>(FractionOfSecond);
			FullSeconds += reminder;
			FractionOfSecond -= reminder;
		}

	public:
		timestamp() : FullSeconds(0), FractionOfSecond(0) {}
		timestamp(std::int64_t seconds, double fraction) :
			FullSeconds(seconds), 
			FractionOfSecond(fraction)
		{
			AdjustFraction();
		}
		timestamp(double fraction) :
			FullSeconds(0), FractionOfSecond(fraction)
		{
			AdjustFraction();
		}
		timestamp(const timestamp& time) :
			FullSeconds(time.FullSeconds), 
			FractionOfSecond(time.FractionOfSecond)
		{
		}

		std::int64_t to_millis() const { return FullSeconds * 1000 + static_cast<std::int64_t>(FractionOfSecond * 1000); }
		double to_double() const { return static_cast<double>(FullSeconds) + FractionOfSecond; }
		operator double() const { return static_cast<double>(FullSeconds) + FractionOfSecond; }
		std::string to_string(std::streamsize precision = 6) const
		{
			std::int64_t full = FullSeconds;
			double fract = FractionOfSecond;

			if (full > 0 && fract < 0)
			{
				full--;
				fract++;
			}
			if (full < 0 && fract > 0)
			{
				full++;
				fract--;
			}

			if (FullSeconds == 0) return std::to_string(FractionOfSecond);
			if (FractionOfSecond == 0) return std::to_string(FullSeconds);
			std::string result = std::to_string(FullSeconds);
			std::ostringstream osfraction;
			osfraction.precision(precision);
			osfraction << FractionOfSecond;
			std::string fraction = osfraction.str();
			if (FractionOfSecond > 0) return result.append(fraction.begin() + 1, fraction.end());
			else result.append(fraction.begin() + 2, fraction.end());
		}

		timestamp operator+(const timestamp& r) const
		{
			timestamp result(FullSeconds, FractionOfSecond);
			result.FullSeconds += r.FullSeconds;
			result.FractionOfSecond += r.FractionOfSecond;
			result.AdjustFraction();
			return result;
		}
		timestamp operator-(const timestamp& r) const
		{
			timestamp result(FullSeconds, FractionOfSecond);
			result.FullSeconds -= r.FullSeconds;
			result.FractionOfSecond -= r.FractionOfSecond;
			result.AdjustFraction();
			return result;
		}
		timestamp& operator=(const timestamp& r)
		{
			FullSeconds = r.FullSeconds;
			FractionOfSecond = r.FractionOfSecond;
			return *this;
		}
		timestamp& operator+=(const timestamp& r)
		{
			FullSeconds += r.FullSeconds;
			FractionOfSecond += r.FractionOfSecond;

			AdjustFraction();

			return *this;
		}
		timestamp& operator-=(const timestamp& r)
		{
			FullSeconds -= r.FullSeconds;
			FractionOfSecond -= r.FractionOfSecond;
			AdjustFraction();
			return *this;
		}
		timestamp& operator++()
		{
			FullSeconds++;
			return *this;
		}
		timestamp& operator--()
		{
			FullSeconds--;
			return *this;
		}
		timestamp operator++(int) 
		{
			timestamp result = *this;
			FullSeconds++;
			return result;
		}
		timestamp operator--(int)
		{
			timestamp result = *this;
			FullSeconds--;
			return result;
		}

		friend timestamp operator+(double v, const timestamp& r)
		{
			timestamp result(r);
			result.FractionOfSecond += v;
			result.AdjustFraction();
			return result;
		}
		friend timestamp operator+(const timestamp& l, double v)
		{
			timestamp result(l);
			result.FractionOfSecond += v;
			result.AdjustFraction();
			return result;
		}
		friend timestamp operator-(double v, const timestamp& r)
		{
			timestamp result(r);
			result.FractionOfSecond -= v;
			result.AdjustFraction();
			return result;
		}
		friend timestamp operator-(const timestamp& l, double v)
		{
			timestamp result(l);
			result.FractionOfSecond -= v;
			result.AdjustFraction();
			return result;
		}
		timestamp& operator=(double v)
		{
			FullSeconds = 0;
			FractionOfSecond = v;
			AdjustFraction();
			return *this;
		}
		timestamp& operator+=(double v)
		{
			FractionOfSecond += v;
			AdjustFraction();
			return *this;
		}
		timestamp& operator-=(double v)
		{
			FractionOfSecond -= v;
			AdjustFraction();
			return *this;
		}

		bool operator==(const timestamp &r) const
		{
			return FullSeconds == r.FullSeconds && FractionOfSecond == r.FractionOfSecond;
		}
		bool operator!=(const timestamp &r) const
		{
			return FullSeconds != r.FullSeconds || FractionOfSecond != r.FractionOfSecond;
		}
		bool operator<(const timestamp &r) const
		{
			timestamp delta = *this - r;
			if (delta.FullSeconds < 0) return true;
			if (delta.FullSeconds > 0) return false;
			return delta.FractionOfSecond < 0;
		}
		bool operator>(const timestamp &r) const
		{
			timestamp delta = *this - r;
			if (delta.FullSeconds > 0) return true;
			if (delta.FullSeconds < 0) return false;
			return delta.FractionOfSecond > 0;
		}
		bool operator<=(const timestamp &r) const
		{
			timestamp delta = *this - r;
			if (delta.FullSeconds < 0) return true;
			if (delta.FullSeconds > 0) return false;
			return delta.FractionOfSecond <= 0;
		}
		bool operator>=(const timestamp &r) const
		{
			timestamp delta = *this - r;
			if (delta.FullSeconds >= 0) return true;
			if (delta.FullSeconds < 0) return false;
			return delta.FractionOfSecond >= 0;
		}
	};
}