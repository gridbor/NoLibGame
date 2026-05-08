#include "ComponentSystem.h"
#include "Transform.h"


namespace components {

	ComponentSystem::ComponentSystem()
	{
		AddComponent<Transform>();
	}

	ComponentSystem::~ComponentSystem()
	{
	}

	void ComponentSystem::Update(float deltaTime)
	{
	}

}
