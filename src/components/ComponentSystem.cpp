#include "ComponentSystem.h"
#include "Transform.h"


namespace components {

	ComponentSystem::ComponentSystem()
	{
		AddComponent<Transform>();
	}

	ComponentSystem::~ComponentSystem()
	{
		for (auto& [_, comp] : m_components) {
			comp.reset();
		}
		m_components.clear();
	}

	void ComponentSystem::Update(float deltaTime)
	{
	}

}
