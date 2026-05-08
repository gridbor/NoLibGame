#pragma once
#include "Component.h"
#include "math/Math.h"


namespace components {

	class Transform : public Component<Transform> {
	public:
		explicit Transform() {}


	private:
		Vector3 m_position{};
		Quaternion m_rotation{};
		Vector3 m_scale{ 1.f };

	};

}
