#pragma once
#include <vector>

#include "graphics/RenderData.h"


namespace text {

	class Symbol {
	public:
		Symbol()
		{
			m_commands.push_back(SymbolCommand(1, Vector2(0.f, 0.f), Vector2(0.5f, 0.1f), Vector2(), Vector3(0.f, 1.f, 1.f)));
			m_commands.push_back(SymbolCommand(0, Vector2(-0.5f, -0.5f), Vector2(0.f, 0.5f), Vector2(), Vector3(1.f, 0.f, 0.f)));
			m_commands.push_back(SymbolCommand(0, Vector2(0.5f, -0.5f), Vector2(0.f, 0.5f), Vector2(), m_color));
			m_commands.push_back(SymbolCommand(0, Vector2(-0.5f, -0.1f), Vector2(0.5f, -0.1f), Vector2(), Vector3(0.f, 0.f, 1.f)));
			m_indices = { 0, 1, 2, 3 };
		}

		~Symbol()
		{
			m_commands.clear();
			m_indices.clear();

			glDeleteBuffers(1, &m_ebo);
			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}

		void Init()
		{
			CreateArrayAndBuffers();

			SetAttribute(0, 1, offsetof(SymbolCommand, opType));
			SetAttribute(1, 2, offsetof(SymbolCommand, v0));
			SetAttribute(2, 2, offsetof(SymbolCommand, v1));
			SetAttribute(3, 2, offsetof(SymbolCommand, v2));
			SetAttribute(4, 3, offsetof(SymbolCommand, color));
		}

		void Render()
		{
			if (m_indices.empty()) return;

			glBindVertexArray(m_vao);
			glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_SHORT, 0);
		}

	private:
		void CreateArrayAndBuffers()
		{
			glCreateVertexArrays(1, &m_vao);
			glCreateBuffers(1, &m_vbo);
			glCreateBuffers(1, &m_ebo);

			glNamedBufferData(m_vbo, m_commands.size() * sizeof(SymbolCommand), m_commands.data(), GL_STATIC_DRAW);
			glNamedBufferData(m_ebo, m_indices.size() * sizeof(uint16_t), m_indices.data(), GL_STATIC_DRAW);

			glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(SymbolCommand));
			glVertexArrayElementBuffer(m_vao, m_ebo);
		}

		void SetAttribute(GLuint index, GLint size, GLsizei offset, GLuint bind_index = 0, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE)
		{
			glEnableVertexArrayAttrib(m_vao, index);
			glVertexArrayAttribFormat(m_vao, index, size, type, normalized, offset);
			glVertexArrayAttribBinding(m_vao, index, bind_index);
		}

	private:
		GLuint m_vao = 0;
		GLuint m_vbo = 0;
		GLuint m_ebo = 0;
		std::vector<SymbolCommand> m_commands;
		std::vector<uint16_t> m_indices;
		Vector3 m_color{ 1.f };

	};

}
