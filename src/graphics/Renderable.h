#pragma once
#include <vector>

#include "GLFunctions.h"
#include "Shaders.h"
#include "RenderData.h"


template<typename T>
class Renderable {
protected:
	Renderable() = default;

public:
	virtual ~Renderable()
	{
		m_vertices.clear();
		m_indices.clear();

		glDeleteBuffers(1, &m_ebo);
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	virtual void Init() = 0;

	virtual void Render()
	{
		if (m_indices.empty()) return;

		Shaders::Get().SetUniformMatrix("uModel", m_modelMatrix);

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	}

	const Matrix4& GetModelMatrix() const { return m_modelMatrix; }

protected:
	void CreateArrayAndBuffers()
	{
		glCreateVertexArrays(1, &m_vao);
		glCreateBuffers(1, &m_vbo);
		glCreateBuffers(1, &m_ebo);

		glNamedBufferData(m_vbo, m_vertices.size() * sizeof(T), m_vertices.data(), GL_STATIC_DRAW);
		glNamedBufferData(m_ebo, m_indices.size() * sizeof(uint16_t), m_indices.data(), GL_STATIC_DRAW);

		glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(T));
		glVertexArrayElementBuffer(m_vao, m_ebo);
	}

	void SetAttribute(GLuint index, GLint size, GLsizei offset, GLuint bind_index = 0, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE)
	{
		glEnableVertexArrayAttrib(m_vao, index);
		glVertexArrayAttribFormat(m_vao, index, size, type, normalized, offset);
		glVertexArrayAttribBinding(m_vao, index, bind_index);
	}

protected:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	std::vector<T> m_vertices;
	std::vector<uint16_t> m_indices;
	Matrix4 m_modelMatrix{};
	Quaternion m_rotation{};
};
