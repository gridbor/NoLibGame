#pragma once
#include "Component.h"
#include "math/Math.h"
#include "utils/Logger.h"


namespace components {

	class Transform : public Component<Transform> {
	public:
		Transform() = default;
		~Transform() = default;

		inline const Vector3& GetPosition() const { return m_position; }
		inline void SetPosition(const Vector3& v)
		{
			m_position = v;
		}
		inline const Quaternion& GetRotation() const { return m_rotation; }
		inline void SetRotation(const Quaternion& q)
		{
			m_rotation = q;
			m_rotation.Normalize();
		}
		inline const Vector3& GetScale() const { return m_scale; }
		inline void SetScale(const Vector3& v)
		{
			m_scale = v;
		}
		inline Matrix4 GetMatrix() const
		{
			Matrix4 mat = m_rotation.ToMatrix4();
			mat.m[0] *= m_scale.x; mat.m[1] *= m_scale.x; mat.m[2] *= m_scale.x;
			mat.m[4] *= m_scale.y; mat.m[5] *= m_scale.y; mat.m[6] *= m_scale.y;
			mat.m[8] *= m_scale.z; mat.m[9] *= m_scale.z; mat.m[10] *= m_scale.z;
			mat.m[12] = m_position.x;
			mat.m[13] = m_position.y;
			mat.m[14] = m_position.z;
			return mat;
		}

	private:
		Vector3 m_position{};
		Quaternion m_rotation{};
		Vector3 m_scale{ 1.f };
	};

}
