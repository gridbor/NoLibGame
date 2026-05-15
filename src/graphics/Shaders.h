#pragma once
#include <unordered_map>
#include <string_view>
#include <memory>

#include "GLFunctions.h"
#include "math/Math.h"


class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	GLuint GetProgramID() const { return m_programID; }
	void CompileShader(GLenum type, const std::string_view& path);
	void LinkGraphicShaders();

private:
	GLuint m_programID = 0;
	std::unordered_map<GLenum, GLuint> m_shaderSources;

};


class Shaders {
public:
	Shaders();
	~Shaders();

	static inline Shaders& Get() { return *s_instance; }

	ShaderProgram* CreateShaderProgram(const std::string_view& name,
		const std::string_view& vertexPath, const std::string_view& fragmentPath,
		const std::string_view& geometryPath = "", const std::string_view& computePath = "");
	ShaderProgram* GetProgram(const std::string_view& name) const;
	void Use(const std::string_view& name);
	void SetUniformMatrix(const std::string_view& uniformName, const Matrix4& mat);

private:
	static inline Shaders* s_instance = nullptr;
	std::unordered_map<std::string_view, std::unique_ptr<ShaderProgram>> m_shaders;
	std::string_view m_currentUse;

};
