#include "Camera.h"
#include "utils/Inputs.h"
#include "components/Transform.h"


Camera::Camera()
{
}

Camera::~Camera()
{
	glDeleteBuffers(1, &m_uniformID);
}

void Camera::Init(float fov, float aspect, float Near, float Far)
{
	m_perspective = Perspective(fov, aspect, Near, Far);
	m_view = LookAt(Vector3(0.f, -3.f, 3.f), Vector3(), UpVector);
	m_data.projection = m_perspective;
	m_data.view = m_view;

	glCreateBuffers(1, &m_uniformID);
	glNamedBufferStorage(m_uniformID, sizeof(CameraData), &m_data, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformID);
}

void Camera::InputProcess()
{
}

void Camera::Update(float deltaTime)
{
	auto tc = GetComponent<components::Transform>();
	Matrix4 move{};
	if (Inputs::Get().IsKeyDown('W')) move.Translate(Vector3(0.f, 1.f, -1.f) * deltaTime);
	if (Inputs::Get().IsKeyDown('A')) move.Translate(Vector3(-1.f, 0.f, 0.f) * deltaTime);
	if (Inputs::Get().IsKeyDown('S')) move.Translate(Vector3(0.f, -1.f, 1.f) * deltaTime);
	if (Inputs::Get().IsKeyDown('D')) move.Translate(Vector3(1.f, 0.f, 0.f) * deltaTime);
	if (Inputs::Get().IsKeyDown('Q')) move.Translate(Vector3(0.f, 0.f, -1.f) * deltaTime);
	if (Inputs::Get().IsKeyDown('E')) move.Translate(Vector3(0.f, 0.f, 1.f) * deltaTime);
	m_view = move * m_view;
	m_data.view = m_view;
}

void Camera::RefreshBuffer()
{
	glNamedBufferSubData(m_uniformID, 0, sizeof(CameraData), &m_data);
}
