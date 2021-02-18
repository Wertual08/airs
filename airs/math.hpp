#pragma once
#include <cstdint>
#include <cmath>
#include <string>
#include <array>



namespace airs
{
	using i8 = std::int8_t;
	using u8 = std::uint8_t;
	using i16 = std::int16_t;
	using u16 = std::uint16_t;
	using i32 = std::int32_t;
	using u32 = std::uint32_t;
	using i64 = std::int64_t;
	using u64 = std::uint64_t;
	using f32 = float;
	using f64 = double;

	template<typename T>
	struct vec2
	{
		T x;
		T y;

		vec2() : x(), y() { }
		explicit vec2(const T &v) : x(v), y(v) { }
		vec2(const T &vx, const T &vy) : x(vx), y(vy) { }
		vec2(const vec2<T> &v) : x(v.x), y(v.y) { }

		vec2<T &> xy() { return { x, y }; }
		vec2<T &> yx() { return { y, x }; }

		vec2<T> xy() const { return { x, y }; }
		vec2<T> yx() const { return { y, x }; }

		template<typename V>
		explicit operator vec2<V>() const { return { static_cast<V>(x), static_cast<V>(y) }; }
		operator vec2<const T &> const() { return { x, y }; }
		operator vec2<T &>() { return { x, y }; }

		vec2 operator+(const vec2 &r) const
		{
			return vec2(x + r.x, y + r.y);
		}
		vec2 operator-(const vec2 &r) const
		{
			return vec2(x - r.x, y - r.y);
		}
		vec2 operator*(const vec2 &r) const
		{
			return vec2(x * r.x, y * r.y);
		}
		vec2 operator/(const vec2 &r) const
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

		vec2 &operator=(const vec2 &vec)
		{
			x = vec.x;
			y = vec.y;
			return *this;
		}
		vec2 &operator+=(const vec2 &vec)
		{
			x += vec.x;
			y += vec.y;
			return *this;
		}
		vec2 &operator-=(const vec2 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			return *this;
		}
		vec2 &operator*=(const vec2 &vec)
		{
			x *= vec.x;
			y *= vec.y;
			return *this;
		}
		vec2 &operator/=(const vec2 &vec)
		{
			x /= vec.x;
			y /= vec.y;
			return *this;
		}

		bool operator==(const vec2 &vec) const
		{
			return x == vec.x && y == vec.y;
		}
		bool operator!=(const vec2 &vec) const
		{
			return x != vec.x || y != vec.y;
		}
		bool operator<(const vec2 &vec) const
		{
			return x < vec.x &&y < vec.y;
		}
		bool operator>(const vec2 &vec) const
		{
			return x > vec.x && y > vec.y;
		}
		bool operator<=(const vec2 &vec) const
		{
			return x <= vec.x && y <= vec.y;
		}
		bool operator>=(const vec2 &vec) const
		{
			return x >= vec.x && y >= vec.y;
		}

		vec2 &operator++()
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
		vec2 &operator--()
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

		template<typename V>
		vec2 rotate(V angle) const
		{
			V cs = std::cos(angle);
			V sn = std::sin(angle);
			return vec2(x * cs - y * sn, x * sn + y * cs);
		}
		vec2 abs() const
		{
			return vec2(std::abs(x), std::abs(y));
		}
		T length() const
		{
			return std::hypot(x, y);
		}
		T sqr() const
		{
			return x * x + y * y;
		}
		T atan2() const
		{
			return std::atan2(y, x);
		}
		operator std::string()
		{
			return std::move("{ " + std::to_string(x) + "; " + std::to_string(y) + " }");
		}

		friend vec2 operator+(const vec2 &vec, const T &val)
		{
			return vec2(vec.x + val, vec.y + val);
		}
		friend vec2 operator-(const vec2 &vec, const T &val)
		{
			return vec2(vec.x - val, vec.y - val);
		}
		friend vec2 operator*(const vec2 &vec, const T &val)
		{
			return vec2(vec.x * val, vec.y * val);
		}
		friend vec2 operator/(const vec2 &vec, const T &val)
		{
			return vec2(vec.x / val, vec.y / val);
		}
		friend vec2 operator+(const T &val, const vec2 &vec)
		{
			return vec2(vec.x + val, vec.y + val);
		}
		friend vec2 operator-(const T &val, const vec2 &vec)
		{
			return vec2(vec.x - val, vec.y - val);
		}
		friend vec2 operator*(const T &val, const vec2 &vec)
		{
			return vec2(vec.x * val, vec.y * val);
		}
		friend vec2 operator/(const T &val, const vec2 &vec)
		{
			return vec2(vec.x / val, vec.y / val);
		}
		vec2 &operator+=(const T &v)
		{
			x += v;
			y += v;
			return *this;
		}
		vec2 &operator-=(const T &v)
		{
			x -= v;
			y -= v;
			return *this;
		}
		vec2 &operator*=(const T &v)
		{
			x *= v;
			y *= v;
			return *this;
		}
		vec2 &operator/=(const T &v)
		{
			x /= v;
			y /= v;
			return *this;
		}
	};
	template<typename T>
	struct vec3
	{
		T x;
		T y;
		T z;

		vec3() : x(), y(), z() { }
		explicit vec3(const T &v) : x(v), y(v), z(v) { }
		vec3(const T &vx, const T &vy, const T &vz) : x(vx), y(vy), z(vz) { }
		vec3(const vec2<T> &v, const T &vz) : x(v.x), y(v.y), z(vz) { }
		vec3(const T &vx, const vec2<T> &v) : x(vx), y(v.x), z(v.y) { }
		vec3(const vec3<T> &v) : x(v.x), y(v.y), z(v.z) { }

		vec2<T &> xy() { return { x, y }; }
		vec2<T &> xz() { return { x, z }; }
		vec2<T &> yx() { return { y, x }; }
		vec2<T &> yz() { return { y, z }; }
		vec2<T &> zx() { return { z, x }; }
		vec2<T &> zy() { return { z, y }; }
		vec3<T &> xyz() { return { x, y, z }; }
		vec3<T &> xzy() { return { x, z, y }; }
		vec3<T &> yxz() { return { y, x, z }; }
		vec3<T &> yzx() { return { y, z, x }; }
		vec3<T &> zxy() { return { z, x, y }; }
		vec3<T &> zyx() { return { z, y, x }; }

		vec2<T> xy() const { return { x, y }; }
		vec2<T> xz() const { return { x, z }; }
		vec2<T> yx() const { return { y, x }; }
		vec2<T> yz() const { return { y, z }; }
		vec2<T> zx() const { return { z, x }; }
		vec2<T> zy() const { return { z, y }; }
		vec3<T> xyz() const { return { x, y, z }; }
		vec3<T> xzy() const { return { x, z, y }; }
		vec3<T> yxz() const { return { y, x, z }; }
		vec3<T> yzx() const { return { y, z, x }; }
		vec3<T> zxy() const { return { z, x, y }; }
		vec3<T> zyx() const { return { z, y, x }; }

		template<typename V>
		explicit operator vec3<V>() const { return { static_cast<V>(x), static_cast<V>(y), static_cast<V>(z) }; }
		operator vec3<const T &> const() { return { x, y, z }; }
		operator vec3<T &>() { return { x, y, z }; }

		vec3 operator+(const vec3 &vec) const
		{
			return vec3(x + vec.x, y + vec.y, z + vec.z);
		}
		vec3 operator-(const vec3 &vec) const
		{
			return vec3(x - vec.x, y - vec.y, z - vec.z);
		}
		vec3 operator*(const vec3 &vec) const
		{
			return vec3(x * vec.x, y * vec.y, z * vec.z);
		}
		vec3 operator/(const vec3 &vec) const
		{
			return vec3(x / vec.x, y / vec.y, z / vec.z);
		}
		vec3 operator+()
		{
			return *this;
		}
		vec3 operator-()
		{
			return vec3(-x, -y, -z);
		}

		vec3 &operator=(const vec3 &vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}
		vec3 &operator+=(const vec3 &vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		vec3 &operator-=(const vec3 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		vec3 &operator*=(const vec3 &vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			return *this;
		}
		vec3 &operator/=(const vec3 &vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			return *this;
		}

		bool operator==(const vec3 &vec) const
		{
			return x == vec.x && y == vec.y && z == vec.z;
		}
		bool operator!=(const vec3 &vec) const
		{
			return x != vec.x || y != vec.y || z != vec.z;
		}
		bool operator<(const vec3 &vec) const
		{
			return x < vec.x &&y < vec.y &&z < vec.z;
		}
		bool operator>(const vec3 &vec) const
		{
			return x > vec.x && y > vec.y && z > vec.z;
		}
		bool operator<=(const vec3 &vec) const
		{
			return x <= vec.x && y <= vec.y && z <= vec.z;
		}
		bool operator>=(const vec3 &vec) const
		{
			return x >= vec.x && y >= vec.y && z >= vec.z;
		}

		vec3 &operator++()
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
		vec3 &operator--()
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

		friend vec3 operator+(const vec3 &vec, const T &val)
		{
			return vec3(vec.x + val, vec.y + val, vec.z + val);
		}
		friend vec3 operator-(const vec3 &vec, const T &val)
		{
			return vec3(vec.x - val, vec.y - val, vec.z - val);
		}
		friend vec3 operator*(const vec3 &vec, const T &val)
		{
			return vec3(vec.x * val, vec.y * val, vec.z * val);
		}
		friend vec3 operator/(const vec3 &vec, const T &val)
		{
			return vec3(vec.x / val, vec.y / val, vec.z / val);
		}
		friend vec3 operator+(const T &val, const vec3 &vec)
		{
			return vec3(vec.x + val, vec.y + val, vec.z + val);
		}
		friend vec3 operator-(const T &val, const vec3 &vec)
		{
			return vec3(vec.x - val, vec.y - val, vec.z - val);
		}
		friend vec3 operator*(const T &val, const vec3 &vec)
		{
			return vec3(vec.x * val, vec.y * val, vec.z * val);
		}
		friend vec3 operator/(const T &val, const vec3 &vec)
		{
			return vec3(vec.x / val, vec.y / val, vec.z / val);
		}
		vec3 &operator+=(const T &v)
		{
			x += v;
			y += v;
			z += v;
			return *this;
		}
		vec3 &operator-=(const T &v)
		{
			x -= v;
			y -= v;
			z -= v;
			return *this;
		}
		vec3 &operator*=(const T &v)
		{
			x *= v;
			y *= v;
			z *= v;
			return *this;
		}
		vec3 &operator/=(const T &v)
		{
			x /= v;
			y /= v;
			z /= v;
			return *this;
		}
	};
	template<typename T>
	struct vec4
	{
		T x;
		T y;
		T z;
		T w;
		vec4() : x(), y(), z(), w() { }
		explicit vec4(const T &v) : x(v), y(v), z(v), w(v) { }
		vec4(const T &vx, const T &vy, const T &vz, const T &vw) : x(vx), y(vy), z(vz), w(vw) { }
		vec4(const vec2<T> &lv, const vec2<T> &rv) : x(lv.x), y(lv.y), z(rv.x), w(rv.y) { }
		vec4(const T &vx, const T &vy, const vec2<T> &v) : x(vx), y(vy), z(v.x), w(v.y) { }
		vec4(const T &vx, const vec2<T> &v, const T &vw) : x(vx), y(v.x), z(v.y), w(vw) { }
		vec4(const vec2<T> &v, const T &vz, const T &vw) : x(v.x), y(v.y), z(vz), w(vw) { }
		vec4(const vec3<T> &v, const T &vw) : x(v.x), y(v.y), z(v.z), w(vw) { }
		vec4(const T &vx, const vec3<T> &v) : x(vx), y(v.x), z(v.y), w(v.z) { }
		vec4(const vec4<T> &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

		vec2<T &> xy() { return { x, y }; }
		vec2<T &> xz() { return { x, z }; }
		vec2<T &> xw() { return { x, w }; }
		vec2<T &> yx() { return { y, x }; }
		vec2<T &> yz() { return { y, z }; }
		vec2<T &> yw() { return { y, w }; }
		vec2<T &> zx() { return { z, x }; }
		vec2<T &> zy() { return { z, y }; }
		vec2<T &> zw() { return { z, w }; }
		vec2<T &> wx() { return { w, x }; }
		vec2<T &> wy() { return { w, y }; }
		vec2<T &> wz() { return { w, z }; }

		vec3<T &> xyz() { return { x, y, z }; }
		vec3<T &> xyw() { return { x, y, w }; }
		vec3<T &> xzy() { return { x, z, y }; }
		vec3<T &> xzw() { return { x, z, w }; }
		vec3<T &> xwy() { return { x, w, y }; }
		vec3<T &> xwz() { return { x, w, z }; }
		vec3<T &> yxz() { return { y, x, z }; }
		vec3<T &> yxw() { return { y, x, w }; }
		vec3<T &> yzx() { return { y, z, x }; }
		vec3<T &> yzw() { return { y, z, w }; }
		vec3<T &> ywx() { return { y, w, x }; }
		vec3<T &> ywz() { return { y, w, z }; }
		vec3<T &> zxy() { return { z, x, y }; }
		vec3<T &> zxw() { return { z, x, w }; }
		vec3<T &> zyx() { return { z, y, x }; }
		vec3<T &> zyw() { return { z, y, w }; }
		vec3<T &> zwx() { return { z, w, x }; }
		vec3<T &> zwy() { return { z, w, y }; }
		vec3<T &> wxy() { return { w, x, y }; }
		vec3<T &> wxz() { return { w, x, z }; }
		vec3<T &> wyx() { return { w, y, x }; }
		vec3<T &> wyz() { return { w, y, z }; }
		vec3<T &> wzx() { return { w, z, x }; }
		vec3<T &> wzy() { return { w, z, y }; }

		vec4<T &> xyzw() { return { x, y, z, w }; }
		vec4<T &> xywz() { return { x, y, w, z }; }
		vec4<T &> xzyw() { return { x, z, y, w }; }
		vec4<T &> xzwy() { return { x, z, w, y }; }
		vec4<T &> xwyz() { return { x, w, y, z }; }
		vec4<T &> xwzy() { return { x, w, z, y }; }
		vec4<T &> yxzw() { return { y, x, z, w }; }
		vec4<T &> yxwz() { return { y, x, w, z }; }
		vec4<T &> yzxw() { return { y, z, x, w }; }
		vec4<T &> yzwx() { return { y, z, w, x }; }
		vec4<T &> ywxz() { return { y, w, x, z }; }
		vec4<T &> ywzx() { return { y, w, z, x }; }
		vec4<T &> zxyw() { return { z, x, y, w }; }
		vec4<T &> zxwy() { return { z, x, w, y }; }
		vec4<T &> zyxw() { return { z, y, x, w }; }
		vec4<T &> zywx() { return { z, y, w, x }; }
		vec4<T &> zwxy() { return { z, w, x, y }; }
		vec4<T &> zwyx() { return { z, w, y, x }; }
		vec4<T &> wxyz() { return { w, x, y, z }; }
		vec4<T &> wxzy() { return { w, x, z, y }; }
		vec4<T &> wyxz() { return { w, y, x, z }; }
		vec4<T &> wyzx() { return { w, y, z, x }; }
		vec4<T &> wzxy() { return { w, z, x, y }; }
		vec4<T &> wzyx() { return { w, z, y, x }; }

		vec2<T> xy() const { return { x, y }; }
		vec2<T> xz() const { return { x, z }; }
		vec2<T> xw() const { return { x, w }; }
		vec2<T> yx() const { return { y, x }; }
		vec2<T> yz() const { return { y, z }; }
		vec2<T> yw() const { return { y, w }; }
		vec2<T> zx() const { return { z, x }; }
		vec2<T> zy() const { return { z, y }; }
		vec2<T> zw() const { return { z, w }; }
		vec2<T> wx() const { return { w, x }; }
		vec2<T> wy() const { return { w, y }; }
		vec2<T> wz() const { return { w, z }; }

		vec3<T> xyz() const { return { x, y, z }; }
		vec3<T> xyw() const { return { x, y, w }; }
		vec3<T> xzy() const { return { x, z, y }; }
		vec3<T> xzw() const { return { x, z, w }; }
		vec3<T> xwy() const { return { x, w, y }; }
		vec3<T> xwz() const { return { x, w, z }; }
		vec3<T> yxz() const { return { y, x, z }; }
		vec3<T> yxw() const { return { y, x, w }; }
		vec3<T> yzx() const { return { y, z, x }; }
		vec3<T> yzw() const { return { y, z, w }; }
		vec3<T> ywx() const { return { y, w, x }; }
		vec3<T> ywz() const { return { y, w, z }; }
		vec3<T> zxy() const { return { z, x, y }; }
		vec3<T> zxw() const { return { z, x, w }; }
		vec3<T> zyx() const { return { z, y, x }; }
		vec3<T> zyw() const { return { z, y, w }; }
		vec3<T> zwx() const { return { z, w, x }; }
		vec3<T> zwy() const { return { z, w, y }; }
		vec3<T> wxy() const { return { w, x, y }; }
		vec3<T> wxz() const { return { w, x, z }; }
		vec3<T> wyx() const { return { w, y, x }; }
		vec3<T> wyz() const { return { w, y, z }; }
		vec3<T> wzx() const { return { w, z, x }; }
		vec3<T> wzy() const { return { w, z, y }; }

		vec4<T> xyzw() const { return { x, y, z, w }; }
		vec4<T> xywz() const { return { x, y, w, z }; }
		vec4<T> xzyw() const { return { x, z, y, w }; }
		vec4<T> xzwy() const { return { x, z, w, y }; }
		vec4<T> xwyz() const { return { x, w, y, z }; }
		vec4<T> xwzy() const { return { x, w, z, y }; }
		vec4<T> yxzw() const { return { y, x, z, w }; }
		vec4<T> yxwz() const { return { y, x, w, z }; }
		vec4<T> yzxw() const { return { y, z, x, w }; }
		vec4<T> yzwx() const { return { y, z, w, x }; }
		vec4<T> ywxz() const { return { y, w, x, z }; }
		vec4<T> ywzx() const { return { y, w, z, x }; }
		vec4<T> zxyw() const { return { z, x, y, w }; }
		vec4<T> zxwy() const { return { z, x, w, y }; }
		vec4<T> zyxw() const { return { z, y, x, w }; }
		vec4<T> zywx() const { return { z, y, w, x }; }
		vec4<T> zwxy() const { return { z, w, x, y }; }
		vec4<T> zwyx() const { return { z, w, y, x }; }
		vec4<T> wxyz() const { return { w, x, y, z }; }
		vec4<T> wxzy() const { return { w, x, z, y }; }
		vec4<T> wyxz() const { return { w, y, x, z }; }
		vec4<T> wyzx() const { return { w, y, z, x }; }
		vec4<T> wzxy() const { return { w, z, x, y }; }
		vec4<T> wzyx() const { return { w, z, y, x }; }

		template<typename V>
		explicit operator vec4<V>() const { return { static_cast<V>(x), static_cast<V>(y), static_cast<V>(z), static_cast<V>(w) }; }
		operator vec4<const T &> const() { return { x, y, z, w }; }
		operator vec4<T &>() { return { x, y, z, w }; }

		vec4 operator+(const vec4 &vec) const
		{
			return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
		vec4 operator-(const vec4 &vec) const
		{
			return vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}
		vec4 operator*(const vec4 &vec) const
		{
			return vec4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
		}
		vec4 operator/(const vec4 &vec) const
		{
			return vec4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
		}
		vec4 operator+()
		{
			return *this;
		}
		vec4 operator-()
		{
			return vec4(-x, -y, -z, -w);
		}

		vec4 &operator=(const vec4 &vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
			return *this;
		}
		vec4 &operator+=(const vec4 &vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			w += vec.w;
			return *this;
		}
		vec4 &operator-=(const vec4 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;
			return *this;
		}
		vec4 &operator*=(const vec4 &vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			w *= vec.w;
			return *this;
		}
		vec4 &operator/=(const vec4 &vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			w /= vec.w;
			return *this;
		}

		bool operator==(const vec4 &vec) const
		{
			return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
		}
		bool operator!=(const vec4 &vec) const
		{
			return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
		}
		bool operator<(const vec4 &vec) const
		{
			return x < vec.x &&y < vec.y &&z < vec.z &&w < vec.w;
		}
		bool operator>(const vec4 &vec) const
		{
			return x > vec.x && y > vec.y && z > vec.z && w > vec.w;
		}
		bool operator<=(const vec4 &vec) const
		{
			return x <= vec.x && y <= vec.y && z <= vec.z && w <= vec.w;
		}
		bool operator>=(const vec4 &vec) const
		{
			return x >= vec.x && y >= vec.y && z >= vec.z && w >= vec.w;
		}

		vec4 &operator++()
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
		vec4 &operator--()
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

		friend vec4 operator+(const vec4 &vec, const T &val)
		{
			return vec4(vec.x + val, vec.y + val, vec.z + val, vec.w + val);
		}
		friend vec4 operator-(const vec4 &vec, const T &val)
		{
			return vec4(vec.x - val, vec.y - val, vec.z - val, vec.w - val);
		}
		friend vec4 operator*(const vec4 &vec, const T &val)
		{
			return vec4(vec.x * val, vec.y * val, vec.z * val, vec.w * val);
		}
		friend vec4 operator/(const vec4 &vec, const T &val)
		{
			return vec4(vec.x / val, vec.y / val, vec.z / val, vec.w / val);
		}
		friend vec4 operator+(const T &val, const vec4 &vec)
		{
			return vec4(vec.x + val, vec.y + val, vec.z + val, vec.w + val);
		}
		friend vec4 operator-(const T &val, const vec4 &vec)
		{
			return vec4(vec.x - val, vec.y - val, vec.z - val, vec.w - val);
		}
		friend vec4 operator*(const T &val, const vec4 &vec)
		{
			return vec4(vec.x * val, vec.y * val, vec.z * val, vec.w * val);
		}
		friend vec4 operator/(const T &val, const vec4 &vec)
		{
			return vec4(vec.x / val, vec.y / val, vec.z / val, vec.w / val);
		}
		vec4 &operator+=(const T &v)
		{
			x += v;
			y += v;
			z += v;
			w += v;
			return *this;
		}
		vec4 &operator-=(const T &v)
		{
			x -= v;
			y -= v;
			z -= v;
			w -= v;
			return *this;
		}
		vec4 &operator*=(const T &v)
		{
			x *= v;
			y *= v;
			z *= v;
			w *= v;
			return *this;
		}
		vec4 &operator/=(const T &v)
		{
			x /= v;
			y /= v;
			z /= v;
			w /= v;
			return *this;
		}
	};

	using vec2i8 = vec2<i8>;
	using vec2u8 = vec2<u8>;
	using vec2i16 = vec2<i16>;
	using vec2u16 = vec2<u16>;
	using vec2i32 = vec2<i32>;
	using vec2u32 = vec2<u32>;
	using vec2i64 = vec2<i64>;
	using vec2u64 = vec2<u64>;
	using vec2f32 = vec2<f32>;
	using vec2f64 = vec2<f64>;

	using vec3i8 = vec3<i8>;
	using vec3u8 = vec3<u8>;
	using vec3i16 = vec3<i16>;
	using vec3u16 = vec3<u16>;
	using vec3i32 = vec3<i32>;
	using vec3u32 = vec3<u32>;
	using vec3i64 = vec3<i64>;
	using vec3u64 = vec3<u64>;
	using vec3f32 = vec3<f32>;
	using vec3f64 = vec3<f64>;

	using vec4i8 = vec4<i8>;
	using vec4u8 = vec4<u8>;
	using vec4i16 = vec4<i16>;
	using vec4u16 = vec4<u16>;
	using vec4i32 = vec4<i32>;
	using vec4u32 = vec4<u32>;
	using vec4i64 = vec4<i64>;
	using vec4u64 = vec4<u64>;
	using vec4f32 = vec4<f32>;
	using vec4f64 = vec4<f64>;

	using vec2b = vec2<std::int8_t>;
	using vec2ub = vec2<std::uint8_t>;
	using vec2s = vec2<std::int16_t>;
	using vec2us = vec2<std::uint16_t>;
	using vec2i = vec2<std::int32_t>;
	using vec2ui = vec2<std::uint32_t>;
	using vec2l = vec2<std::int64_t>;
	using vec2ul = vec2<std::uint64_t>;
	using vec2f = vec2<float>;
	using vec2d = vec2<double>;

	using vec3b = vec3<std::int8_t>;
	using vec3ub = vec3<std::uint8_t>;
	using vec3s = vec3<std::int16_t>;
	using vec3us = vec3<std::uint16_t>;
	using vec3i = vec3<std::int32_t>;
	using vec3ui = vec3<std::uint32_t>;
	using vec3l = vec3<std::int64_t>;
	using vec3ul = vec3<std::uint64_t>;
	using vec3f = vec3<float>;
	using vec3d = vec3<double>;

	using vec4b = vec4<std::int8_t>;
	using vec4ub = vec4<std::uint8_t>;
	using vec4s = vec4<std::int16_t>;
	using vec4us = vec4<std::uint16_t>;
	using vec4i = vec4<std::int32_t>;
	using vec4ui = vec4<std::uint32_t>;
	using vec4l = vec4<std::int64_t>;
	using vec4ul = vec4<std::uint64_t>;
	using vec4f = vec4<float>;
	using vec4d = vec4<double>;

	template<typename T>
	struct mat2
	{
		struct row
		{
			T v[2];
			inline float &operator[](int i) { return v[i]; }
		};
		row m[2];
		inline row &operator[](int i) { return m[i]; }
		const static mat2 identity;
	};
	template<typename T>
	const mat2<T> mat2<T>::identity{ 1.0f, 0.0f, 0.0f, 1.0f };

	struct mat3
	{
		struct row
		{
			float v[3];
			inline float &operator[](int i) { return v[i]; }
		};
		row m[3];
		inline row &operator[](int i) { return m[i]; }
		const static mat3 identity;
	};
	struct mat4
	{
		struct row
		{
			float v[4];
			inline float &operator[](int i) { return v[i]; }
		};
		row m[4];
		inline row &operator[](int i) { return m[i]; }
		const static mat4 identity;
	};

	using mat2f = mat2<float>;


	class noise
	{
	private:
		std::array<uint8_t, 512> Permutation;

	public:
		noise();
		noise(uint32_t seed);
		double operator()(double x, double y, double z) const;
		double operator()(vec3d vec) const;
	};
}
