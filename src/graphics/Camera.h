#pragma once
#include "graphics/RenderData.h"
#include "GLFunctions.h"
#include "components/ComponentSystem.h"


class Camera : public components::ComponentSystem {
public:
	Camera();
	virtual ~Camera();

	virtual void Init(float fov, float aspect, float Near, float Far);

	const Matrix4& GetPerspective() const { return m_perspective; }
	const Matrix4& GetView() const { return m_view; }
	const CameraData& GetData() const { return m_data; }
	GLuint GetUniformID() const { return m_uniformID; }

	virtual void InputProcess();
	virtual void Update(float deltaTime);
	virtual void RefreshBuffer();

protected:
	Matrix4 m_perspective{};
	Matrix4 m_view{};
	GLuint m_uniformID = 0;
	CameraData m_data{};

};
