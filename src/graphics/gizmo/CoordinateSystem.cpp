#include "CoordinateSystem.h"


namespace gizmo {

	CoordinateSystem::CoordinateSystem()
	{
		float s = 0.06f;
		float ps = 2.f * s;
		float w = 1.f - (2.f * ps);
		float hs = s * 0.5f;
		for (size_t a = 0; a < 3; a++) {
			size_t vcount = m_vertices.size();
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
				float co = i == 12 ? 0.6f : (im % 2 == 0 ? 0.f : 0.2f);
				if (a < 1) {
					m_vertices.push_back(SimpleVertex{ Vector3(v.y, v.x, v.z), Vector3(1.f - co, 0.f, 0.f) });
				}
				else if (a < 2) {
					m_vertices.push_back(SimpleVertex{ Vector3(v.x, v.y, v.z), Vector3(0.f, 1.f - co, 0.f) });
				}
				else {
					m_vertices.push_back(SimpleVertex{ Vector3(v.x, v.z, v.y), Vector3(0.f, 0.f, 1.f - co) });
				}
			}
			m_indices.insert(m_indices.end(), {
				(unsigned short)(vcount + 0), (unsigned short)(vcount + (a == 1 ? 1 : 5)), (unsigned short)(vcount + (a == 1 ? 5 : 1)), (unsigned short)(vcount + 0), (unsigned short)(vcount + (a == 1 ? 5 : 4)), (unsigned short)(vcount + (a == 1 ? 4 : 5)),
				(unsigned short)(vcount + 3), (unsigned short)(vcount + (a == 1 ? 0 : 4)), (unsigned short)(vcount + (a == 1 ? 4 : 0)), (unsigned short)(vcount + 3), (unsigned short)(vcount + (a == 1 ? 4 : 7)), (unsigned short)(vcount + (a == 1 ? 7 : 4)),
				(unsigned short)(vcount + 2), (unsigned short)(vcount + (a == 1 ? 7 : 6)), (unsigned short)(vcount + (a == 1 ? 6 : 7)), (unsigned short)(vcount + 2), (unsigned short)(vcount + (a == 1 ? 3 : 7)), (unsigned short)(vcount + (a == 1 ? 7 : 3)),
				(unsigned short)(vcount + 1), (unsigned short)(vcount + (a == 1 ? 6 : 5)), (unsigned short)(vcount + (a == 1 ? 5 : 6)), (unsigned short)(vcount + 1), (unsigned short)(vcount + (a == 1 ? 2 : 6)), (unsigned short)(vcount + (a == 1 ? 6 : 2)),
				(unsigned short)(vcount + 8), (unsigned short)(vcount + (a == 1 ? 10 : 9)), (unsigned short)(vcount + (a == 1 ? 9 : 10)), (unsigned short)(vcount + 8), (unsigned short)(vcount + (a == 1 ? 11 : 10)), (unsigned short)(vcount + (a == 1 ? 10 : 11)),
				(unsigned short)(vcount + 8), (unsigned short)(vcount + (a == 1 ? 9 : 12)), (unsigned short)(vcount + (a == 1 ? 12 : 9)),
				(unsigned short)(vcount + 8), (unsigned short)(vcount + (a == 1 ? 12 : 11)), (unsigned short)(vcount + (a == 1 ? 11 : 12)),
				(unsigned short)(vcount + 11), (unsigned short)(vcount + (a == 1 ? 12 : 10)), (unsigned short)(vcount + (a == 1 ? 10 : 12)),
				(unsigned short)(vcount + 9), (unsigned short)(vcount + (a == 1 ? 10 : 12)), (unsigned short)(vcount + (a == 1 ? 12 : 10))
			});
		}
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
