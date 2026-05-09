#pragma once
#include "graphics/RenderData.h"
#include "GLFunctions.h"
#include "components/ComponentSystem.h"


class Camera : public components::ComponentSystem {
public:
	Camera() = default;
	Camera(Vector3 position, Vector3 eulerAngles);
	virtual ~Camera();

	virtual void Init(float fov, float aspect, float Near, float Far);

	const Matrix4& GetPerspective() const { return m_data.projection; }
	const Matrix4& GetView() const { return m_data.view; }
	const CameraData& GetData() const { return m_data; }
	GLuint GetUniformID() const { return m_uniformID; }

	virtual void InputProcess();
	virtual void Update(float deltaTime);
	virtual void RefreshBuffer();

protected:
	GLuint m_uniformID = 0;
	CameraData m_data{};
	// In Radians
	Vector3 m_eulerAngles{};

};
