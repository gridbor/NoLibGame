#pragma once
#include "GLFunctions.h"
#include "math/Math.h"


namespace lights {

	enum class ELightType : unsigned char {
		Directional = 0,
		Spherical,
		Conical
	};


	class Light {
	public:
		Light() : m_direction{ 0.f, 1.f, -1.f } {};
		Light(const Vector3& direction) : m_direction{ direction } {}
		virtual ~Light() = default;

		virtual void ApplyToProgram(GLuint programID, GLsizei location)
		{
			glProgramUniform3fv(programID, location, 1, &m_direction.v[0]);
		}

	protected:
		ELightType m_type = ELightType::Directional;
		Vector3 m_direction{};

	};

}
