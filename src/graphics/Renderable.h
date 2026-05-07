#pragma once
#include <vector>

#include "GLFunctions.h"
#include "RenderData.h"


class Renderable {
protected:
	Renderable() = default;

public:
	virtual ~Renderable()
	{
	}

	virtual void Init()
	{
		glCreateVertexArrays(1, &m_vao);
		glCreateBuffers(1, &m_vbo);
		glCreateBuffers(1, &m_ebo);

		glNamedBufferData(m_vbo, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
		glNamedBufferData(m_ebo, m_indices.size() * sizeof(uint32_t), m_indices.data(), GL_STATIC_DRAW);

		glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));
		glVertexArrayElementBuffer(m_vao, m_ebo);

		glEnableVertexArrayAttrib(m_vao, 0);
		glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
		glVertexArrayAttribBinding(m_vao, 0, 0);

		glEnableVertexArrayAttrib(m_vao, 1);
		glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
		glVertexArrayAttribBinding(m_vao, 1, 0);

		glEnableVertexArrayAttrib(m_vao, 2);
		glVertexArrayAttribFormat(m_vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
		glVertexArrayAttribBinding(m_vao, 2, 0);

		glEnableVertexArrayAttrib(m_vao, 3);
		glVertexArrayAttribFormat(m_vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
		glVertexArrayAttribBinding(m_vao, 3, 0);
	}

	virtual void Render()
	{
		if (m_indices.empty()) return;

		Shaders::Get().SetUniformMatrix("uModel", m_modelMatrix);

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	const Matrix4& GetModelMatrix() const { return m_modelMatrix; }

protected:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	Matrix4 m_modelMatrix{};
};
