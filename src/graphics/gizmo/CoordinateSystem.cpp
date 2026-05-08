#include "CoordinateSystem.h"


namespace gizmo {

	CoordinateSystem::CoordinateSystem()
	{
		float s = 0.1f;
		float ps = 2.f * s;
		float w = 1.f - (2.f * ps);
		float hs = s * 0.5f;
		for (size_t i = 0; i < 13; i++) {
			Vector3 v{};
			size_t im = i % 4;
			if (i != 12) {
				v.x = (i < 8 ? hs : (ps * 0.5f)) * (im < 2 ? 1.f : -1.f);
				v.y = (i < 4 ? (hs * (i == 2 ? -1.f : 1.f)) : w);
				v.z = (i < 8 ? hs : (ps * 0.5f)) * (im == 1 || im == 2 ? -1.f : 1.f);
			}
			else {
				v.x = 0.f;
				v.y = 1.f;
				v.z = 0.f;
			}
			float co = (float)im * 0.2f;
			m_vertices.push_back(SimpleVertex{ v, Vector3{ 0.f, 1.f - co, 0.f}});
		}
		m_indices = {
			0, 5, 1, 0, 4, 5,
			3, 4, 0, 3, 7, 4,
			2, 6, 7, 2, 7, 3,
			1, 5, 6, 1, 6, 2,
			8, 9, 10, 8, 10, 11,
			8, 12, 9,
			8, 11, 12,
			11, 10, 12,
			9, 12, 10
		};
	}

	CoordinateSystem::~CoordinateSystem()
	{
	}

	void CoordinateSystem::Init()
	{
		CreateArrayAndBuffers();

		SetAttribute(0, 3, offsetof(SimpleVertex, position));
		SetAttribute(1, 3, offsetof(SimpleVertex, color));
	}

	void CoordinateSystem::Update(float deltaTime)
	{
	}

	void CoordinateSystem::Render()
	{
		if (m_indices.empty()) return;

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	}

}
