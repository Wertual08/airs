#pragma once
#include <cstdint>
#include <string>
#include <vector>



namespace airs
{
	template<typename T>
	struct vec2
	{
		union
		{
			T x;
			T r;
			T s;
		};
		union
		{
			T y;
			T g;
			T t;
		};
		vec2()
		{
			x = y = 0;
		}
		vec2(T v)
		{
			this->x = v;
			this->y = v;
		}
		vec2(T x, T y)
		{
			this->x = x;
			this->y = y;
		}
		template<typename V0, typename V1>
		vec2(V0 x, V1 y)
		{
			this->x = static_cast<T>(x);
			this->y = static_cast<T>(y);
		}
		template<typename V>
		vec2(const vec2<V>& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
		}

		vec2 operator+(const vec2& r) const
		{
			return vec2(x + r.x, y + r.y);
		}
		vec2 operator-(const vec2& r) const
		{
			return vec2(x - r.x, y - r.y);
		}
		vec2 operator*(const vec2& r) const
		{
			return vec2(x * r.x, y * r.y);
		}
		vec2 operator/(const vec2& r) const
		{
			return vec2(x / r.x, y / r.y);
		}
		vec2 operator+() const
		{
			return *this;
		}
		vec2 operator-() const
		{
			return vec2(-x, -y);
		}

		vec2& operator=(const vec2& r)
		{
			x = r.x;
			y = r.y;
			return *this;
		}
		vec2& operator+=(const vec2& r)
		{
			x += r.x;
			y += r.y;
			return *this;
		}
		vec2& operator-=(const vec2& r)
		{
			x -= r.x;
			y -= r.y;
			return *this;
		}
		vec2& operator*=(const vec2& r)
		{
			x *= r.x;
			y *= r.y;
			return *this;
		}
		vec2& operator/=(const vec2& r)
		{
			x /= r.x;
			y /= r.y;
			return *this;
		}

		bool operator==(const vec2& r) const
		{
			return x == r.x && y == r.y;
		}
		bool operator!=(const vec2& r) const
		{
			return x != r.x || y != r.y;
		}
		bool operator<(const vec2& r) const
		{
			return x < r.x && y < r.y;
		}
		bool operator>(const vec2& r) const
		{
			return x > r.x && y > r.y;
		}
		bool operator<=(const vec2& r) const
		{
			return x <= r.x && y <= r.y;
		}
		bool operator>=(const vec2& r) const
		{
			return x >= r.x&& y >= r.y;
		}

		vec2& operator++()
		{
			x++;
			y++;
			return *this;
		}
		vec2 operator++(int)
		{
			vec2 temp = *this;
			x++;
			y++;
			return temp;
		}
		vec2& operator--()
		{
			x--;
			y--;
			return *this;
		}
		vec2 operator--(int)
		{
			vec2 temp = *this;
			x--;
			y--;
			return temp;
		}

		operator T*()
		{
			return static_cast<T*>(static_cast<void*>(this));
		}
		operator const T* () const
		{
			return static_cast<T*>(static_cast<void*>(this));
		}

		template<typename V>
		vec2 rotate(V angle) const
		{
			V cs = cos(angle);
			V sn = sin(angle);
			return vec2(x * cs - y * sn, x * sn + y * cs);
		}
		vec2 abs() const
		{
			return vec2(::abs(x), ::abs(y));
		}
		operator std::string()
		{
			return std::move("{ " + std::to_string(x) + "; " + std::to_string(y) + " }");
		}
	};
	template<typename T>
	struct vec3
	{
		union
		{
			T x;
			T r;
			T s;
		};
		union
		{
			T y;
			T g;
			T t;
		};
		union
		{
			T z;
			T b;
			T p;
		};
		vec3()
		{
			x = y = z = 0;
		}
		vec3(T v)
		{
			this->x = v;
			this->y = v;
			this->z = v;
		}
		vec3(T x, T y)
		{
			this->x = x;
			this->y = y;
			this->z = 0;
		}
		vec3(T x, T y, T z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		template<typename V0, typename V1, typename V2>
		vec3(V0 x, V1 y, V2 z)
		{
			this->x = static_cast<T>(x);
			this->y = static_cast<T>(y);
			this->z = static_cast<T>(z);
		}
		template<typename V>
		vec3(const vec2<V>& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			z = 0;
		}
		template<typename V>
		vec3(const vec3<V>& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			z = static_cast<T>(v.z);
		}

		vec3 operator+(const vec3& r) const
		{
			return vec3(x + r.x, y + r.y, z + r.z);
		}
		vec3 operator-(const vec3& r) const
		{
			return vec3(x - r.x, y - r.y, z - r.z);
		}
		vec3 operator*(const vec3& r) const
		{
			return vec3(x * r.x, y * r.y, z * r.z);
		}
		vec3 operator/(const vec3& r) const
		{
			return vec3(x / r.x, y / r.y, z / r.z);
		}
		vec3 operator+()
		{
			return *this;
		}
		vec3 operator-()
		{
			return vec3(-x, -y, -z);
		}

		vec3& operator=(const vec3& r)
		{
			x = r.x;
			y = r.y;
			z = r.z;
			return *this;
		}
		vec3& operator+=(const vec3& r)
		{
			x += r.x;
			y += r.y;
			z += r.z;
			return *this;
		}
		vec3& operator-=(const vec3& r)
		{
			x -= r.x;
			y -= r.y;
			z -= r.z;
			return *this;
		}
		vec3& operator*=(const vec3& r)
		{
			x *= r.x;
			y *= r.y;
			z *= r.z;
			return *this;
		}
		vec3& operator/=(const vec3& r)
		{
			x /= r.x;
			y /= r.y;
			z /= r.z;
			return *this;
		}

		bool operator==(const vec3& r) const
		{
			return x == r.x && y == r.y && z == r.z;
		}
		bool operator!=(const vec3& r) const
		{
			return x != r.x || y != r.y || z != r.z;
		}
		bool operator<(const vec3& r) const
		{
			return x < r.x && y < r.y && z < r.z;
		}
		bool operator>(const vec3& r) const
		{
			return x > r.x && y > r.y && z > r.z;
		}
		bool operator<=(const vec3& r) const
		{
			return x <= r.x && y <= r.y && z <= r.z;
		}
		bool operator>=(const vec3& r) const
		{
			return x >= r.x && y >= r.y && z >= r.z;
		}

		vec3& operator++()
		{
			x++;
			y++;
			z++;
			return *this;
		}
		vec3 operator++(int)
		{
			vec3 temp = *this;
			x++;
			y++;
			z++;
			return temp;
		}
		vec3& operator--()
		{
			x--;
			y--;
			z--;
			return *this;
		}
		vec3 operator--(int)
		{
			vec3 temp = *this;
			x--;
			y--;
			z--;
			return temp;
		}

		operator T* ()
		{
			return static_cast<T*>(static_cast<void*>(this));
		}
		operator const T* () const
		{
			return static_cast<T*>(static_cast<void*>(this));
		}

		vec3 abs() const
		{
			return vec3(::abs(x), ::abs(y), ::abs(z));
		}
	};
	template<typename T>
	struct vec4
	{
		union
		{
			T x;
			T r;
			T s;
		};
		union
		{
			T y;
			T g;
			T t;
		};
		union
		{
			T z;
			T b;
			T p;
		};
		union
		{
			T w;
			T a;
			T q;
		};
		vec4()
		{
			x = y = z = w = 0;
		}
		vec4(T v)
		{
			this->x = v;
			this->y = v;
			this->z = v;
			this->w = v;
		}
		vec4(T x, T y)
		{
			this->x = x;
			this->y = y;
			this->z = 0;
			this->w = 0;
		}
		vec4(T x, T y, T z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = 0;
		}
		vec4(T x, T y, T z, T w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		template<typename V0, typename V1, typename V2, typename V3>
		vec4(V0 x, V1 y, V2 z, V3 w)
		{
			this->x = static_cast<T>(x);
			this->y = static_cast<T>(y);
			this->z = static_cast<T>(z);
			this->w = static_cast<T>(w);
		}
		template<typename V>
		vec4(const vec2<V>& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			z = 0;
			w = 0;
		}
		template<typename V>
		vec4(const vec3<V>& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			z = static_cast<T>(v.z);
			w = 0;
		}
		template<typename V>
		vec4(const vec4<V>& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			z = static_cast<T>(v.z);
			w = static_cast<T>(v.w);
		}

		vec4 operator+(const vec4& r) const
		{
			return vec4(x + r.x, y + r.y, z + r.z, w + r.w);
		}
		vec4 operator-(const vec4& r) const
		{
			return vec4(x - r.x, y - r.y, z - r.z, w - r.w);
		}
		vec4 operator*(const vec4& r) const
		{
			return vec4(x * r.x, y * r.y, z * r.z, w * r.w);
		}
		vec4 operator/(const vec4& r) const
		{
			return vec4(x / r.x, y / r.y, z / r.z, w / r.w);
		}
		vec4 operator+()
		{
			return *this;
		}
		vec4 operator-()
		{
			return vec4(-x, -y, -z, -w);
		}

		vec4& operator=(const vec4& r)
		{
			x = r.x;
			y = r.y;
			z = r.z;
			w = r.w;
			return *this;
		}
		vec4& operator+=(const vec4& r)
		{
			x += r.x;
			y += r.y;
			z += r.z;
			w += r.w;
			return *this;
		}
		vec4& operator-=(const vec4& r)
		{
			x -= r.x;
			y -= r.y;
			z -= r.z;
			w -= r.w;
			return *this;
		}
		vec4& operator*=(const vec4& r)
		{
			x *= r.x;
			y *= r.y;
			z *= r.z;
			w *= r.w;
			return *this;
		}
		vec4& operator/=(const vec4& r)
		{
			x /= r.x;
			y /= r.y;
			z /= r.z;
			w /= r.w;
			return *this;
		}

		bool operator==(const vec4& r) const
		{
			return x == r.x && y == r.y && z == r.z && w == r.w;
		}
		bool operator!=(const vec4& r) const
		{
			return x != r.x || y != r.y || z != r.z || w != r.w;
		}
		bool operator<(const vec4& r) const
		{
			return x < r.x && y < r.y && z < r.z && w < r.w;
		}
		bool operator>(const vec4& r) const
		{
			return x > r.x&& y > r.y && z > r.z && w > r.w;
		}
		bool operator<=(const vec4& r) const
		{
			return x <= r.x && y <= r.y && z <= r.z && w <= r.w;
		}
		bool operator>=(const vec4& r) const
		{
			return x >= r.x && y >= r.y && z >= r.z && w >= r.w;
		}

		vec4& operator++()
		{
			x++;
			y++;
			z++;
			w++;
			return *this;
		}
		vec4 operator++(int)
		{
			vec4 temp = *this;
			x++;
			y++;
			z++;
			w++;
			return temp;
		}
		vec4& operator--()
		{
			x--;
			y--;
			z--;
			w--;
			return *this;
		}
		vec4 operator--(int)
		{
			vec4 temp = *this;
			x--;
			y--;
			z--;
			w--;
			return temp;
		}

		operator T* ()
		{
			return static_cast<T*>(static_cast<void*>(this));
		}
		operator const T* ()
		{
			return static_cast<T*>(static_cast<void*>(this));
		}

		vec4 abs() const
		{
			return vec4(::abs(x), ::abs(y), ::abs(z), ::abs(w));
		}
	};

	using vec2b = vec2<int8_t>;
	using vec2ub = vec2<uint8_t>;
	using vec2s = vec2<int16_t>;
	using vec2us = vec2<uint16_t>;
	using vec2i = vec2<int32_t>;
	using vec2ui = vec2<uint32_t>;
	using vec2l = vec2<int64_t>;
	using vec2ul = vec2<uint64_t>;
	using vec2f = vec2<float>;
	using vec2d = vec2<double>;

	using vec3b = vec3<int8_t>;
	using vec3ub = vec3<uint8_t>;
	using vec3s = vec3<int16_t>;
	using vec3us = vec3<uint16_t>;
	using vec3i = vec3<int32_t>;
	using vec3ui = vec3<uint32_t>;
	using vec3l = vec3<int64_t>;
	using vec3ul = vec3<uint64_t>;
	using vec3f = vec3<float>;
	using vec3d = vec3<double>;

	using vec4b = vec4<int8_t>;
	using vec4ub = vec4<uint8_t>;
	using vec4s = vec4<int16_t>;
	using vec4us = vec4<uint16_t>;
	using vec4i = vec4<int32_t>;
	using vec4ui = vec4<uint32_t>;
	using vec4l = vec4<int64_t>;
	using vec4ul = vec4<uint64_t>;
	using vec4f = vec4<float>;
	using vec4d = vec4<double>;

	struct mat2
	{
		struct row
		{
			float v[2];
			inline float& operator[](int i) { return v[i]; }
		};
		row m[2];
		inline row& operator[](int i) { return m[i]; }
		const static mat2 identity;
	};
	struct mat3
	{
		struct row
		{
			float v[3];
			inline float& operator[](int i) { return v[i]; }
		};
		row m[3];
		inline row& operator[](int i) { return m[i]; }
		const static mat3 identity;
	};
	struct mat4
	{
		struct row
		{
			float v[4];
			inline float& operator[](int i) { return v[i]; }
		};
		row m[4];
		inline row& operator[](int i) { return m[i]; }
		const static mat4 identity;
	};

	class PerlinNoise
	{
		std::vector<uint8_t> Permutation;

	public:
		PerlinNoise();
		PerlinNoise(uint32_t seed);
	};
}
