#pragma once
#include "Renderable.h"
#include "utils/Logger.h"
#include "utils/Inputs.h"


class Texture;

class Plane : public Renderable<Vertex> {
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
