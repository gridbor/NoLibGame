#pragma once
#include <cmath>
#include <limits>


inline const float Pi = 3.14159265358979323846f;

inline float ToRadians(float degree) { return degree * Pi / 180.f; }
inline float ToDegree(float radians) { return radians * 180.f / Pi; }

inline bool NearlyEquals(float a, float b, float epsilon = std::numeric_limits<float>::epsilon()) { return std::fabsf(a - b) < epsilon; }
inline float Clamp(float t, float min = 0.f, float max = 1.f) { return t < min ? min : (t > max ? max : t); }


#include "Vectors.h"
#include "Matrices.h"


inline const Vector3 ZeroVector = Vector3{};
inline const Vector3 UpVector = Vector3{ 0.f, 0.f, 1.f };
inline const Vector3 ForwardVector = Vector3{ 1.f, 0.f, 0.f };
inline const Vector3 LeftVector = Vector3{ 0.f, 1.f, 0.f };

inline float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}
inline float Dot(const Vector4& v1, const Vector4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }
inline Vector4 Cross(const Vector4& v1, const Vector4& v2)
{
	return Vector4(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
		0.f
	);
}

inline Matrix4 Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
	Matrix4 result{ 0.f };
	result.m00 = 2.f / (Right - Left);
	result.m11 = 2.f / (Top - Bottom);
	result.m22 = -(2.f / (Far - Near));
	result.m4[0] = -((Right + Left) / (Right - Left));
	result.m4[1] = -((Top + Bottom) / (Top - Bottom));
	result.m4[2] = -((Far + Near) / (Far - Near));
	return result;
}

inline Matrix4 Perspective(float FOVDegree, float Aspect, float Near, float Far)
{
	float FOVRadians = FOVDegree * Pi / 180.f;
	float tanHalfFOV = tanf(FOVRadians * 0.5f);
	Matrix4 result{ 0.f };
	result.m[0] = 1.f / (Aspect * tanHalfFOV);
	result.m[5] = 1.f / tanHalfFOV;
	result.m[10] = -(Far + Near) / (Far - Near);
	result.m[11] = -1.f;
	result.m[14] = -(2.f * Far * Near) / (Far - Near);
	return result;
}

inline Matrix4 LookAt(Vector3 eye, Vector3 center, Vector3 worldUp)
{
	Vector3 forward = Vector3::Normalize(center - eye);
	Vector3 right = Vector3::Normalize(Cross(forward, worldUp));
	Vector3 up = Cross(right, forward);
	Matrix4 result{};
	result.m[0] = right.x; result.m[4] = right.y; result.m[8] = right.z;
	result.m[1] = up.x; result.m[5] = up.y; result.m[9] = up.z;
	result.m[2] = -forward.x; result.m[6] = -forward.y; result.m[10] = -forward.z;
	result.m[12] = -Dot(right, eye);
	result.m[13] = -Dot(up, eye);
	result.m[14] = Dot(forward, eye);
	return result;
}


#include "Quaternion.h"


inline float Dot(const Quaternion& q1, const Quaternion& q2) { return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z; }
inline Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	Quaternion result;
	float invT = 1.f - t;
	result.w = q1.w * invT + q2.w * t;
	result.x = q1.x * invT + q2.x * t;
	result.y = q1.y * invT + q2.y * t;
	result.z = q1.z * invT + q2.z * t;
	result.Normalize();
	return result;
}
inline Quaternion Slerp(Quaternion q1, Quaternion q2, float t)
{
	float cosTheta = Dot(q1, q2);
	if (cosTheta < 0.f) {
		q2 = q2 * -1.f;
		cosTheta = -cosTheta;
	}
	if (NearlyEquals(cosTheta, 1.f, 0.0001f)) {
		return Lerp(q1, q2, t);
	}
	float angle = std::acosf(cosTheta);
	float sinTheta = std::sinf(angle);
	float t0 = std::sinf((1.f - t) * angle) / sinTheta;
	float t1 = std::sinf(t * angle) / sinTheta;
	return Quaternion(
		q1.w * t0 + q2.w * t1,
		q1.x * t0 + q2.x * t1,
		q1.y * t0 + q2.y * t1,
		q1.z * t0 + q2.z * t1
	);
}
inline Quaternion QuaternionFromMatrix(const Matrix4& mat)
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


inline void MatrixDecompose(const Matrix4& M, Vector3& position, Quaternion& rotation, Vector3& scale)
{
	position.x = M.m30; position.y = M.m31; position.z = M.m32;
	scale.x = std::sqrtf(M.m[0] * M.m[0] + M.m[1] * M.m[1] + M.m[2] * M.m[2]);
	scale.y = std::sqrtf(M.m[4] * M.m[4] + M.m[5] * M.m[5] + M.m[6] * M.m[6]);
	scale.z = std::sqrtf(M.m[8] * M.m[8] + M.m[9] * M.m[9] + M.m[10] * M.m[10]);
	Matrix4 cm{ M.m };
	for (int i = 0; i < 3; i++) {
		bool sz = NearlyEquals(scale.v[i], 0.f);
		float invScale = sz ? 0.f : (1.f / scale.v[i]);
		if (sz) {
			cm.m[i * 4 + 0] = i == 0 ? 1.f : 0.f;
			cm.m[i * 4 + 1] = i == 1 ? 1.f : 0.f;
			cm.m[i * 4 + 2] = i == 2 ? 1.f : 0.f;
		}
		else {
			cm.m[i * 4 + 0] *= invScale;
			cm.m[i * 4 + 1] *= invScale;
			cm.m[i * 4 + 2] *= invScale;
		}
	}
	rotation = QuaternionFromMatrix(cm);
}
