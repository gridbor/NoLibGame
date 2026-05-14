#pragma once
#include "WorldObject.h"


enum class EWorldObjectType : unsigned char {
	Plane = 0
};

struct TransformData {
	Vector3 position{};
	Quaternion rotation{};
	Vector3 scale{ 1.f };
};

struct WorldObjectProperty {
	EWorldObjectType type;
	TransformData transform;
	WorldObject* owner = nullptr;
};


class World : public WorldObject {
public:
	World();
	~World();

	virtual void Init() override;
	virtual void Render() override;

	virtual void CreateWorldObject(const WorldObjectProperty& property);

private:
	std::unordered_map<std::string_view, std::unique_ptr<WorldObject>> m_objects;

};
