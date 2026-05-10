#pragma once
#include "Renderable.h"
#include "utils/Logger.h"
#include "utils/Inputs.h"


class Plane : public Renderable<Vertex> {
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
		CreateArrayAndBuffers();

		SetAttribute(0, 3, offsetof(Vertex, position));
		SetAttribute(1, 3, offsetof(Vertex, color));
		SetAttribute(2, 3, offsetof(Vertex, normal));
		SetAttribute(3, 2, offsetof(Vertex, uv));
	}

	virtual void Render() override
	{
		if (Inputs::Get().IsMouseButtonPressed(EMouseButton::Left)) {
			auto transform = GetComponent<components::Transform>();
			if (transform != nullptr) {
				transform->RotateAxis(UpVector, 0.1f);
			}
		}

		Renderable::Render();
	}
};
