#include "Plane.h"
#include "Texture.h"


Plane::Plane() : Plane(Vector3(1.f)) {}

Plane::Plane(const Vector3& color) : m_color{ color }
{
	m_vertices = {
		Vertex{
			.position = Vector3(-0.5f, 0.5f, 0.f),
			.color = m_color,
			.normal = UpVector,
			.uv = Vector2(0.f, 1.f)
		},
		Vertex{
			.position = Vector3(0.5f, 0.5f, 0.f),
			.color = m_color,
			.normal = UpVector,
			.uv = Vector2(0.f, 0.f)
		},
		Vertex{
			.position = Vector3(0.5f, -0.5f, 0.f),
			.color = m_color,
			.normal = UpVector,
			.uv = Vector2(1.f, 0.f)
		},
		Vertex{
			.position = Vector3(-0.5f, -0.5f, 0.f),
			.color = m_color,
			.normal = UpVector,
			.uv = Vector2(1.f, 1.f)
		}
	};
	m_indices = { 0, 2, 1, 0, 3, 2 };
	m_texture = std::make_unique<Texture>();
}

Plane::~Plane()
{
	m_texture.reset();
}

void Plane::Init()
{
	CreateArrayAndBuffers();

	SetAttribute(0, 3, offsetof(Vertex, position));
	SetAttribute(1, 3, offsetof(Vertex, color));
	SetAttribute(2, 3, offsetof(Vertex, normal));
	SetAttribute(3, 2, offsetof(Vertex, uv));

	m_texture->Init(Assets::Get().LoadTextureData("images/test.png"));
}

void Plane::Render()
{
	glBindTextureUnit(0, m_texture->GetTextureID());
	Renderable::Render();
}
