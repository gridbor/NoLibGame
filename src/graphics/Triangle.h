#pragma once
#include "Renderable.h"


class Triangle : public Renderable {
public:
	Triangle()
	{
		m_vertices.push_back(Vertex{ Vector3(-0.5f, -0.5f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(), Vector2() });
		m_vertices.push_back(Vertex{ Vector3(0.0f,  0.5f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(), Vector2() });
		m_vertices.push_back(Vertex{ Vector3(0.5f, -0.5f, 0.f), Vector3(0.f, 0.f, 1.f), Vector3(), Vector2() });
		m_indices = { 0, 2, 1 };
	}

	~Triangle()
	{
	}

	virtual void Init() override
	{
		Renderable::Init();
	}

	virtual void Render() override
	{
		Renderable::Render();
	}

private:

};
