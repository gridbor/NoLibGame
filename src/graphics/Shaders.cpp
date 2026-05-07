#include "Shaders.h"
#include "assets/Assets.h"
#include "utils/Logger.h"


ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_programID);
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
	glAttachShader(m_programID, m_shaderSources[GL_VERTEX_SHADER]);
	glAttachShader(m_programID, m_shaderSources[GL_FRAGMENT_SHADER]);
	glLinkProgram(m_programID);

	GLint success;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
		LLog("ERROR: Shader program linkage failed! {}", infoLog);
	}
	glDeleteShader(m_shaderSources[GL_VERTEX_SHADER]);
	glDeleteShader(m_shaderSources[GL_FRAGMENT_SHADER]);
	m_shaderSources.erase(GL_VERTEX_SHADER);
	m_shaderSources.erase(GL_FRAGMENT_SHADER);
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

ShaderProgram* Shaders::CreateShaderProgram(const std::string_view& name, const std::string_view& vertexPath, const std::string_view& fragmentPath)
{
	if (m_shaders.contains(name)) {
		return m_shaders[name].get();
	}

	std::unique_ptr<ShaderProgram> program = std::make_unique<ShaderProgram>();
	program->CompileShader(GL_VERTEX_SHADER, vertexPath);
	program->CompileShader(GL_FRAGMENT_SHADER, fragmentPath);
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
