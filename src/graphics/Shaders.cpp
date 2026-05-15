#include "Shaders.h"
#include "assets/Assets.h"
#include "utils/Logger.h"


ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	if (!m_shaderSources.empty()) m_shaderSources.clear();
	if (m_programID != 0) glDeleteProgram(m_programID);
}

void ShaderProgram::CompileShader(GLenum type, const std::string_view& path)
{
	GLuint shader = glCreateShader(type);
	std::string source = Assets::Get().LoadText(path);
	const char* str = source.c_str();
	glShaderSource(shader, 1, &str, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		LLog("ERROR: Shader compilation failed! {}", infoLog);
	}
	m_shaderSources[type] = shader;
}

void ShaderProgram::LinkGraphicShaders()
{
	m_programID = glCreateProgram();
	for (const auto& [_, source] : m_shaderSources) {
		glAttachShader(m_programID, source);
	}
	glLinkProgram(m_programID);

	GLint success;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
		LLog("ERROR: Shader program linkage failed! {}", infoLog);
	}

	for (auto& [_, source] : m_shaderSources) {
		glDeleteShader(source);
	}
	m_shaderSources.clear();
}




Shaders::Shaders()
{
	s_instance = this;
}

Shaders::~Shaders()
{
	for (auto& [_, program] : m_shaders) {
		program.reset();
	}
	m_shaders.clear();
}

ShaderProgram* Shaders::CreateShaderProgram(const std::string_view& name,
	const std::string_view& vertexPath, const std::string_view& fragmentPath,
	const std::string_view& geometryPath, const std::string_view& computePath)
{
	if (m_shaders.contains(name)) {
		return m_shaders[name].get();
	}

	std::unique_ptr<ShaderProgram> program = std::make_unique<ShaderProgram>();
	program->CompileShader(GL_VERTEX_SHADER, vertexPath);
	program->CompileShader(GL_FRAGMENT_SHADER, fragmentPath);
	if (!geometryPath.empty()) program->CompileShader(GL_GEOMETRY_SHADER, geometryPath);
	if (!computePath.empty()) program->CompileShader(GL_COMPUTE_SHADER, computePath);
	program->LinkGraphicShaders();
	m_shaders[name] = std::move(program);
	return m_shaders[name].get();
}

ShaderProgram* Shaders::GetProgram(const std::string_view& name) const
{
	if (m_shaders.contains(name)) {
		return m_shaders.at(name).get();
	}
	return nullptr;
}

void Shaders::Use(const std::string_view& name)
{
	if (!m_shaders.contains(name)) return;
	glUseProgram(m_shaders.at(name)->GetProgramID());
	m_currentUse = name;
}

void Shaders::SetUniformMatrix(const std::string_view& uniformName, const Matrix4& mat)
{
	if (m_currentUse.empty()) return;
	GLint uniformLocation = glGetUniformLocation(m_shaders[m_currentUse]->GetProgramID(), uniformName.data());
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, mat.m);
}
