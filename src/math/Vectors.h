#pragma once
#include <string>


struct Vector2 {
	union {
		struct { float x, y; };
		struct { float r, g; };
		struct { float s, t; };
		float v[2];
	};

	Vector2() : x{ 0.f }, y{ 0.f } {}
	Vector2(float x, float y) : x{ x }, y{ y } {}
	Vector2(float s) : x{ s }, y{ s } {}

	inline Vector2 operator-() const { return Vector2(-x, -y); }

	inline Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator*(const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	inline Vector2 operator/(const Vector2& v) const { return Vector2(x / v.x, y / v.y); }

	inline Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	inline Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	inline Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
	inline Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }

	inline Vector2& operator-=(float s) { x -= s; y -= s; return *this; }
	inline Vector2& operator+=(float s) { x += s; y += s; return *this; }
	inline Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
	inline Vector2& operator/=(float s) { x /= s; y /= s; return *this; }

	inline float Length() const { return std::sqrtf(x * x + y * y); }
	inline float LenghtSqr() const { return x * x + y * y; }

	static inline Vector2 Normalize(const Vector2& v)
	{
		float l = v.Length();
		if (l > 0.f) {
			float invl = 1.f / l;
			return v * invl;
		}
		return Vector2();
	}

	inline std::string ToString() const { return "{ X=" + std::to_string(x) + ", Y=" + std::to_string(y) + " }"; }
};


struct Vector3 {
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float s, t, p; };
		struct { float yaw, pitch, roll; };
		float v[3];
	};

	Vector3() : x{ 0.f }, y{ 0.f }, z{ 0.f } {}
	Vector3(const Vector2& v, float z) : x{ v.x }, y{ v.y }, z{ z } {}
	Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
	Vector3(float s) : x{ s }, y{ s }, z{ s } {}

	inline Vector3 operator-() const { return Vector3(-x, -y, -z); }

	inline Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
	inline Vector3 operator/(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

	inline Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	inline Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	inline Vector3& operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

	inline Vector3& operator-=(float s) { x -= s; y -= s; z -= s; return *this; }
	inline Vector3& operator+=(float s) { x += s; y += s; z += s; return *this; }
	inline Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	inline Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

	inline float Length() const { return std::sqrtf(x * x + y * y + z * z); }
	inline float LenghtSqr() const { return x * x + y * y + z * z; }
	inline float Dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
	inline Vector3 Cross(const Vector3& v) const
	{
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}

	inline Vector3& Normalize()
	{
		float l = Length();
		if (l > 0.f) {
			*this *= 1.f / l;
		}
		return *this;
	}

	static inline Vector3 Normalize(const Vector3& v)
	{
		float l = v.Length();
		if (l > 0.f) {
			float invl = 1.f / l;
			return v * invl;
		}
		return Vector3();
	}

	inline std::string ToString() const { return "{ X=" + std::to_string(x) + ", Y=" + std::to_string(y) + ", Z=" + std::to_string(z) + " }"; }
};


struct Vector4 {
	union {
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		struct { float s, t, p, q; };
		float v[4];
	};

	Vector4() : x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 0.f } {}
	Vector4(const Vector3& v, float w) : x{ v.x }, y{ v.y }, z{ v.z }, w{ w } {}
	Vector4(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w } {}
	Vector4(float s) : x{ s }, y{ s }, z{ s }, w{ s } {}

	inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

	inline Vector4 operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline Vector4 operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Vector4 operator*(const Vector4& v) const { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
	inline Vector4 operator/(const Vector4& v) const { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); }

	inline Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	inline Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	inline Vector4& operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	inline Vector4& operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	inline Vector4& operator-=(float s) { x -= s; y -= s; z -= s; w -= s; return *this; }
	inline Vector4& operator+=(float s) { x += s; y += s; z += s; w += s; return *this; }
	inline Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	inline Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

	inline float Length() const { return std::sqrtf(x * x + y * y + z * z + w * w); }
	inline float LenghtSqr() const { return x * x + y * y + z * z + w * w; }
	inline float Dot(const Vector4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	inline Vector4 Cross(const Vector4& v) const
	{
		return Vector4(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x,
			0.f
		);
	}

	static inline Vector4 Normalize(const Vector4& v)
	{
		float l = v.Length();
		if (l > 0.f) {
			float invl = 1.f / l;
			return v * invl;
		}
		return Vector4();
	}

	inline std::string ToString() const { return "{ X=" + std::to_string(x) + ", Y=" + std::to_string(y) + ", Z=" + std::to_string(z) + ", W=" + std::to_string(w) + " }"; }
};
