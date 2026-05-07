#pragma once
#include "Renderable.h"
#include "utils/Logger.h"
#include "utils/Inputs.h"


class Plane : public Renderable {
public:
	Plane()
	{
		m_vertices.push_back(Vertex{ Vector3(-0.5f, -0.5f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(), Vector2() });
		m_vertices.push_back(Vertex{ Vector3(-0.5f,  0.5f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(), Vector2() });
		m_vertices.push_back(Vertex{ Vector3(0.5f, 0.5f, 0.f), Vector3(0.f, 0.f, 1.f), Vector3(), Vector2() });
		m_vertices.push_back(Vertex{ Vector3(0.5f, -0.5f, 0.f), Vector3(1.f, 1.f, 0.f), Vector3(), Vector2() });
		m_indices = { 0, 2, 1, 0, 3, 2 };
	}

	~Plane()
	{
	}

	virtual void Init() override
	{
		Renderable::Init();
	}

	virtual void Render() override
	{
		Renderable::Render();

		if (Inputs::Get().IsMouseButtonPressed(EMouseButton::Left)) {
			
		}
	}
};
