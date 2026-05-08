#pragma once


struct Quaternion {
	union {
		struct { float w, x, y, z; };
		float v[4];
	};

	Quaternion() : w{ 1.f }, x{ 0.f }, y{ 0.f }, z{ 0.f } {}
	Quaternion(float w, float x, float y, float z) : w{ w }, x{ x }, y{ y }, z{ z } {}
	// from axis angle 
	Quaternion(Vector3 axis, float angleRadians)
	{
		float halfAngle = angleRadians * 0.5f;
		float s = std::sinf(halfAngle);
		w = std::cosf(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}

	inline Quaternion operator+(const Quaternion& q) const { return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z); }
	inline Quaternion& operator+=(const Quaternion& q) { w += q.w; x += q.x; y += q.y; z += q.z; return *this; }
	inline Quaternion operator-(const Quaternion& q) const { return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z); }
	inline Quaternion& operator-=(const Quaternion& q) { w -= q.w; x -= q.x; y -= q.y; z -= q.z; return *this; }
	inline Quaternion operator*(const Quaternion& q) const
	{
		return Quaternion(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w
		);
	}
	inline Quaternion& operator*=(const Quaternion& q)
	{
		Quaternion t{ w, x, y, z };
		w = t.w * q.w - t.x * q.x - t.y * q.y - t.z * q.z;
		x = t.w * q.x + t.x * q.w + t.y * q.z - t.z * q.y;
		y = t.w * q.y - t.x * q.z + t.y * q.w + t.z * q.x;
		z = t.w * q.z + t.x * q.y - t.y * q.x + t.z * q.w;
		return *this;
	}
	inline Quaternion operator*(float s) const { return Quaternion(w * s, x * s, y * s, z * s); }
	inline Quaternion& operator*=(float s) { w *= s; x *= s; y *= s; z *= s; return *this; }
	inline Quaternion operator/(float s) const { return Quaternion(w / s, x / s, y / s, z / s); }
	inline Quaternion& operator/=(float s) { w /= s; x /= s; y /= s; z /= s; return *this; }

	inline float LengthSqr() const { return w * w + x * x + y * y + z * z; }
	inline float Length() const { return std::sqrtf(LengthSqr()); }
	inline Quaternion Conjugate() const { return Quaternion(w, -x, -y, -z); }
	inline Quaternion Inverse() const
	{
		float ls = LengthSqr();
		if (ls > 0.0001f) {
			float invLs = 1.f / ls;
			return Conjugate() * invLs;
		}
		return Quaternion();
	}
	inline void Normalize()
	{
		float l = Length();
		*this /= l;
	}
	inline bool Equals(const Quaternion& q, float epsilon = std::numeric_limits<float>::epsilon()) const
	{
		return NearlyEquals(w, q.w, epsilon)
			&& NearlyEquals(x, q.x, epsilon)
			&& NearlyEquals(y, q.y, epsilon)
			&& NearlyEquals(z, q.z, epsilon);
	}

	inline Matrix4 ToMatrix4() const
	{
		Matrix4 mat{};
		mat.m00 = 1.f - 2.f * (y * y + z * z);
		mat.m01 = 2.f * (x * y + w * z);
		mat.m02 = 2.f * (x * z - w * y);

		mat.m10 = 2.f * (x * y - w * z);
		mat.m11 = 1.f - 2.f * (x * x + z * z);
		mat.m12 = 2.f * (y * z + w * x);

		mat.m20 = 2.f * (x * z + w * y);
		mat.m21 = 2.f * (y * z - w * x);
		mat.m22 = 1.f - 2.f * (x * x + y * y);
		return mat;
	}

	static inline Quaternion FromMatrix(const Matrix4& mat)
	{
		Quaternion q{};
		float tr = mat.m00 + mat.m11 + mat.m22;
		float s;
		if (tr > 0) {
			s = std::sqrtf(tr + 1.f) * 2.f;
			q.w = 0.25f * s;
			q.x = (mat.m12 - mat.m21) / s;
			q.y = (mat.m20 - mat.m02) / s;
			q.z = (mat.m01 - mat.m10) / s;
		}
		else if ((mat.m00 > mat.m11) && (mat.m00 > mat.m22)) {
			s = std::sqrtf(1.f + mat.m00 - mat.m11 - mat.m22) * 2.f;
			q.w = (mat.m12 - mat.m21) / s;
			q.x = 0.25f * s;
			q.y = (mat.m10 + mat.m01) / s;
			q.z = (mat.m20 + mat.m02) / s;
		}
		else if (mat.m11 > mat.m22) {
			s = std::sqrtf(1.f + mat.m11 - mat.m00 - mat.m22) * 2.f;
			q.w = (mat.m20 - mat.m02) / s;
			q.x = (mat.m10 + mat.m01) / s;
			q.y = 0.25f * s;
			q.z = (mat.m21 + mat.m12) / s;
		}
		else {
			s = std::sqrtf(1.f + mat.m22 - mat.m00 - mat.m11) * 2.f;
			q.w = (mat.m01 - mat.m10) / s;
			q.x = (mat.m20 + mat.m02) / s;
			q.y = (mat.m21 + mat.m12) / s;
			q.z = 0.25f * s;
		}
		return q;
	}
};
