#pragma once
#include "objects/WorldObject.h"


class Texture;

class Plane : public WorldObject {
public:
	Plane();
	Plane(const Vector3& color);
	~Plane();

	virtual void Init() override;
	virtual void Render() override;

private:
	Vector3 m_color{ 1.f };
	std::unique_ptr<Texture> m_texture;

};
