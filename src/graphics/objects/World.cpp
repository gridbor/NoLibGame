#include "World.h"
#include "Plane.h"


World::World()
{
}

World::~World()
{
}

void World::Init()
{
}

void World::Render()
{
	WorldObject::Render();
}

void World::CreateWorldObject(const WorldObjectProperty& property)
{
	WorldObject* obj = nullptr;
	switch (property.type) {
	case EWorldObjectType::Plane:
		obj = new Plane();
		break;
	}
	if (obj == nullptr) return;
	AddChild(obj);
	if (components::Transform* t = obj->GetComponent<components::Transform>()) {
		t->SetPosition(property.transform.position);
		t->SetRotation(property.transform.rotation);
		t->SetScale(property.transform.scale);
	}
	obj->Init();
	m_objects["WorldObject_" + std::to_string(obj->GetID())].reset(obj);
}
