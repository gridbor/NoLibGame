#pragma once
#include "Vectors.h"
#include "Matrices.h"


inline const float Pi = 3.14159265358979323846f;

inline const Vector3 ZeroVector = Vector3{};
inline const Vector3 UpVector = Vector3{ 0.f, 0.f, 1.f };
inline const Vector3 ForwardVector = Vector3{ 1.f, 0.f, 0.f };
inline const Vector3 RightVector = Vector3{ 0.f, 1.f, 0.f };

inline bool NearlyEquals(float a, float b, float epsilon = std::numeric_limits<float>::epsilon()) { return std::fabsf(a - b) < epsilon; }

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
