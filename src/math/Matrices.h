#pragma once
#include <string>


struct Matrix4 {
	union {
		struct {
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		struct {
			float m1[4];
			float m2[4];
			float m3[4];
			float m4[4];
		};
		float m[16];
	};

	Matrix4() : m{ 1.f, 0.f, 0.f, 0.f,    0.f, 1.f, 0.f, 0.f,    0.f, 0.f, 1.f, 0.f,    0.f, 0.f, 0.f, 1.f } {}
	Matrix4(const float d[16])
	{
		for (size_t i = 0; i < 16; i++) m[i] = d[i];
	}
	Matrix4(std::initializer_list<float> list)
	{
		for (size_t i = 0; i < 16; i++) {
			if (i < list.size()) {
				m[i] = *(list.begin() + i);
			}
			else {
				m[i] = 0.f;
			}
		}
	}
	explicit Matrix4(float s) : Matrix4{}
	{
		m00 = s;
		m11 = s;
		m22 = s;
		m33 = s;
	}

	inline Matrix4 operator*(const Matrix4& M) const
	{
		Matrix4 mul = Matrix4(0.f);
		float A00 = m[0], A10 = m[1], A20 = m[2], A30 = m[3];
		float A01 = m[4], A11 = m[5], A21 = m[6], A31 = m[7];
		float A02 = m[8], A12 = m[9], A22 = m[10], A32 = m[11];
		float A03 = m[12], A13 = m[13], A23 = m[14], A33 = m[15];
		for (size_t i = 0; i < 4; i++) {
			float B0 = M.m[i * 4 + 0];
			float B1 = M.m[i * 4 + 1];
			float B2 = M.m[i * 4 + 2];
			float B3 = M.m[i * 4 + 3];
			mul.m[i * 4 + 0] = A00 * B0 + A01 * B1 + A02 * B2 + A03 * B3;
			mul.m[i * 4 + 1] = A10 * B0 + A11 * B1 + A12 * B2 + A13 * B3;
			mul.m[i * 4 + 2] = A20 * B0 + A21 * B1 + A22 * B2 + A23 * B3;
			mul.m[i * 4 + 3] = A30 * B0 + A31 * B1 + A32 * B2 + A33 * B3;
		}
		return mul;
	}
	inline Matrix4& operator*=(const Matrix4& M)
	{
		float A00 = m[0], A10 = m[1], A20 = m[2], A30 = m[3];
		float A01 = m[4], A11 = m[5], A21 = m[6], A31 = m[7];
		float A02 = m[8], A12 = m[9], A22 = m[10], A32 = m[11];
		float A03 = m[12], A13 = m[13], A23 = m[14], A33 = m[15];
		for (size_t i = 0; i < 4; i++) {
			float B0 = M.m[i * 4 + 0];
			float B1 = M.m[i * 4 + 1];
			float B2 = M.m[i * 4 + 2];
			float B3 = M.m[i * 4 + 3];
			m[i * 4 + 0] = A00 * B0 + A01 * B1 + A02 * B2 + A03 * B3;
			m[i * 4 + 1] = A10 * B0 + A11 * B1 + A12 * B2 + A13 * B3;
			m[i * 4 + 2] = A20 * B0 + A21 * B1 + A22 * B2 + A23 * B3;
			m[i * 4 + 3] = A30 * B0 + A31 * B1 + A32 * B2 + A33 * B3;
		}
		return *this;
	}

	inline Vector4 operator*(const Vector4& v) const
	{
		return Vector4(
			m00 * v.x + m10 * v.y + m20 * v.z + m30 * v.w,
			m01 * v.x + m11 * v.y + m21 * v.z + m31 * v.w,
			m02 * v.x + m12 * v.y + m22 * v.z + m32 * v.w,
			m03 * v.x + m13 * v.y + m23 * v.z + m33 * v.w
		);
	}

	inline Matrix4& Translate(const Vector3& v) { return *this *= Translation(v); }
	static inline Matrix4 Translation(float x, float y, float z) { return Translation(Vector3(x, y, z)); }
	static inline Matrix4 Translation(const Vector3& v)
	{
		Matrix4 result{};
		result.m[12] = v.x;
		result.m[13] = v.y;
		result.m[14] = v.z;
		return result;
	}

	inline Matrix4& Scale(const Vector3& v) { return *this *= Scaling(v); }
	static inline Matrix4 Scaling(float s) { return Scaling(Vector3(s)); }
	static inline Matrix4 Scaling(float x, float y, float z) { return Scaling(Vector3(x, y, z)); }
	static inline Matrix4 Scaling(const Vector3& v)
	{
		Matrix4 result{};
		result.m[0] = v.x;
		result.m[5] = v.y;
		result.m[10] = v.z;
		return result;
	}

	inline Matrix4 Invert(bool ignoreScale = false) const
	{
		Matrix4 inv{};
		Vector3 isv{ 1.f };
		if (!ignoreScale) {
			float sx = m[0] * m[0] + m[1] * m[1] + m[2] * m[2];
			float sy = m[4] * m[4] + m[5] * m[5] + m[6] * m[6];
			float sz = m[8] * m[8] + m[9] * m[9] + m[10] * m[10];
			isv.x = NearlyEquals(sx, 0.f) ? 0.f : 1.f / sx;
			isv.y = NearlyEquals(sy, 0.f) ? 0.f : 1.f / sy;
			isv.z = NearlyEquals(sz, 0.f) ? 0.f : 1.f / sz;
		}
		inv.m[0] = m[0] * isv.x; inv.m[1] = m[4] * isv.x; inv.m[2] = m[8] * isv.x;
		inv.m[4] = m[1] * isv.y; inv.m[5] = m[5] * isv.y; inv.m[6] = m[9] * isv.y;
		inv.m[8] = m[2] * isv.z; inv.m[9] = m[6] * isv.z; inv.m[10] = m[10] * isv.z;
		inv.m[12] = -(inv.m[0] * m[12] + inv.m[4] * m[13] + inv.m[8] * m[14]);
		inv.m[13] = -(inv.m[1] * m[12] + inv.m[5] * m[13] + inv.m[9] * m[14]);
		inv.m[14] = -(inv.m[2] * m[12] + inv.m[6] * m[13] + inv.m[10] * m[14]);
		return inv;
	}

	inline std::string ToString() const
	{
		std::string s = "[ ";
		for (size_t i = 0; i < 16; i++) {
			if (i != 0 && i % 4 == 0) s += "\n  ";
			s += std::to_string(m[i]) + (i != 15 ? ", " : "");
		}
		s += " ]";
		return s;
	}
};
