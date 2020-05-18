#include "airs/math.h"
#include <algorithm>
#include <numeric>
#include <random>



namespace airs
{
	const mat2 mat2::identity{ 1.0f, 0.0f, 0.0f, 1.0f };
	const mat3 mat3::identity{ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	const mat4 mat4::identity{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };


	inline double fade(double t) 
	{
		return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
	}
	inline double lerp(double t, double a, double b) 
	{
		return a + t * (b - a);
	}
	inline double grad(int32_t h, double x, double y, double z) 
	{
		h &= 15;
		double u = h < 8 ? x : y;
		double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}
	noise::noise()
	{
		Permutation = {
			151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
			8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
			35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
			134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
			55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
			18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
			250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
			189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,
			43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,
			97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
			107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
			151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
			8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
			35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
			134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
			55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
			18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
			250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
			189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,
			43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,
			97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
			107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
		};
	}
	noise::noise(uint32_t seed)
	{
		std::iota(Permutation.begin(), Permutation.end(), 0);
		std::shuffle(Permutation.begin(), Permutation.end(), std::mt19937(seed));
		std::memcpy(Permutation.data() + 256, Permutation.data(), 256);
	}
	double noise::operator()(double x, double y, double z)
	{
		int32_t X = (int32_t)floor(x) & 255;
		int32_t Y = (int32_t)floor(y) & 255;
		int32_t Z = (int32_t)floor(z) & 255;

		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		double u = fade(x);
		double v = fade(y);
		double w = fade(z);

		int32_t A = Permutation[X] + Y;
		int32_t AA = Permutation[A] + Z;
		int32_t AB = Permutation[A + 1] + Z;
		int32_t B = Permutation[X + 1] + Y;
		int32_t BA = Permutation[B] + Z;
		int32_t BB = Permutation[B + 1] + Z;

		return (lerp(w, 
			lerp(v, 
				lerp(u, 
					grad(Permutation[AA], x, y, z), 
					grad(Permutation[BA], x - 1, y, z)),
				lerp(u, 
					grad(Permutation[AB], x, y - 1, z), 
					grad(Permutation[BB], x - 1, y - 1, z))),
			lerp(v, 
				lerp(u, 
					grad(Permutation[AA + 1], x, y, z - 1), 
					grad(Permutation[BA + 1], x - 1, y, z - 1)),
				lerp(u, 
					grad(Permutation[AB + 1], x, y - 1, z - 1), 
					grad(Permutation[BB + 1], x - 1, y - 1, z - 1)))
		) + 1.0) / 2.0;
	}
	double noise::operator()(vec3d vec)
	{
		int32_t X = (int32_t)floor(vec.x) & 255;
		int32_t Y = (int32_t)floor(vec.y) & 255;
		int32_t Z = (int32_t)floor(vec.z) & 255;

		vec.x -= floor(vec.x);
		vec.y -= floor(vec.y);
		vec.z -= floor(vec.z);

		double u = fade(vec.x);
		double v = fade(vec.y);
		double w = fade(vec.z);

		int32_t A = Permutation[X] + Y;
		int32_t AA = Permutation[A] + Z;
		int32_t AB = Permutation[A + 1] + Z;
		int32_t B = Permutation[X + 1] + Y;
		int32_t BA = Permutation[B] + Z;
		int32_t BB = Permutation[B + 1] + Z;

		return (lerp(w,
			lerp(v,
				lerp(u,
					grad(Permutation[AA], vec.x, vec.y, vec.z),
					grad(Permutation[BA], vec.x - 1, vec.y, vec.z)),
				lerp(u,
					grad(Permutation[AB], vec.x, vec.y - 1, vec.z),
					grad(Permutation[BB], vec.x - 1, vec.y - 1, vec.z))),
			lerp(v,
				lerp(u,
					grad(Permutation[AA + 1], vec.x, vec.y, vec.z - 1),
					grad(Permutation[BA + 1], vec.x - 1, vec.y, vec.z - 1)),
				lerp(u,
					grad(Permutation[AB + 1], vec.x, vec.y - 1, vec.z - 1),
					grad(Permutation[BB + 1], vec.x - 1, vec.y - 1, vec.z - 1)))
		) + 1.0) / 2.0;
	}
}