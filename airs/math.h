#pragma once
#include <cstdint>
#include <string>
#include <array>



namespace airs
{
	using i8 = int8_t;
	using u8 = uint8_t;
	using i16 = int16_t;
	using u16 = uint16_t;
	using i32 = int32_t;
	using u32 = uint32_t;
	using i64 = int64_t;
	using u64 = uint64_t;
	using f32 = float;
	using f64 = double;

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
		vec2() : x(), y() { }
		vec2(T v) : x(v), y(v) { }
		vec2(T vx, T vy) : x(vx), y(vy) { }
		template<typename V0, typename V1>
		vec2(V0 vx, V1 vy) : x(static_cast<T>(vx)), y(static_cast<T>(vy)) { }
		template<typename V>
		vec2(const vec2<V>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }

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
		T length() const
		{
			return ::hypot(x, y);
		}
		T sqr() const
		{
			return x * x + y * y;
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
		vec3() : x(), y(), z() { }
		vec3(T v) : x(v), y(v), z(v) { }
		vec3(T vx, T vy) : x(vx), y(vy), z() { }
		vec3(T vx, T vy, T vz) : x(vx), y(vy), z(vz) { }
		template<typename V0, typename V1, typename V2>
		vec3(V0 vx, V1 vy, V2 vz) : x(static_cast<T>(vx)), y(static_cast<T>(vy)), z(static_cast<T>(vz)) { }
		template<typename V>
		vec3(const vec2<V>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z() { }
		template<typename V>
		vec3(const vec3<V>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) { }

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
		T length() const
		{
			return sqrt(x * x + y * y + z * z);
		}
		T sqr() const
		{
			return x * x + y * y + z * z;
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
		vec4() : x(), y(), z(), w() { }
		vec4(T v) : x(v), y(v), z(v), w(v) { }
		vec4(T vx, T vy) : x(vx), y(vy), z(), w() { }
		vec4(T vx, T vy, T vz) : x(vx), y(vy), z(vz), w() { }
		vec4(T vx, T vy, T vz, T vw) : x(vx), y(vy), z(vz), w(vw) { }
		template<typename V0, typename V1, typename V2, typename V3>
		vec4(V0 vx, V1 vy, V2 vz, V3 vw) : x(static_cast<T>(vx)), y(static_cast<T>(vy)), z(static_cast<T>(vz)), w(static_cast<T>(vw)) { }
		template<typename V>
		vec4(const vec2<V>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(), w() { }
		template<typename V>
		vec4(const vec3<V>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w() { }
		template<typename V> 
		vec4(const vec4<V>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) { }

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
		T length() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}
		T sqr() const
		{
			return x * x + y * y + z * z + w * w;
		}
	};

	using vec2i8  = vec2<i8>;
	using vec2u8  = vec2<u8>;
	using vec2i16 = vec2<i16>;
	using vec2u16 = vec2<u16>;
	using vec2i32 = vec2<i32>;
	using vec2u32 = vec2<u32>;
	using vec2i64 = vec2<i64>;
	using vec2u64 = vec2<u64>;
	using vec2f32 = vec2<f32>;
	using vec2f64 = vec2<f64>;

	using vec3i8  = vec3<i8>;
	using vec3u8  = vec3<u8>;
	using vec3i16 = vec3<i16>;
	using vec3u16 = vec3<u16>;
	using vec3i32 = vec3<i32>;
	using vec3u32 = vec3<u32>;
	using vec3i64 = vec3<i64>;
	using vec3u64 = vec3<u64>;
	using vec3f32 = vec3<f32>;
	using vec3f64 = vec3<f64>;

	using vec4i8  = vec4<i8>;
	using vec4u8  = vec4<u8>;
	using vec4i16 = vec4<i16>;
	using vec4u16 = vec4<u16>;
	using vec4i32 = vec4<i32>;
	using vec4u32 = vec4<u32>;
	using vec4i64 = vec4<i64>;
	using vec4u64 = vec4<u64>;
	using vec4f32 = vec4<f32>;
	using vec4f64 = vec4<f64>;

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

	class noise
	{
	private:
		std::array<uint8_t, 512> Permutation;

	public:
		noise();
		noise(uint32_t seed);
		double operator()(double x, double y, double z);
		double operator()(vec3d vec);
	};
}
