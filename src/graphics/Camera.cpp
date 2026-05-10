#include "Camera.h"
#include "utils/Inputs.h"
#include "components/Transform.h"


Camera::Camera(Vector3 position, Vector3 eulerAngles) : m_eulerAngles{ eulerAngles }
{
	auto t = GetComponent<components::Transform>();
	if (t) {
		t->SetPosition(position);
		t->SetRotation(Quaternion(m_eulerAngles));
	}
}

Camera::~Camera()
{
	glDeleteBuffers(1, &m_uniformID);
}

void Camera::Init(float fov, float aspect, float Near, float Far)
{
	m_data.projection = Perspective(fov, aspect, Near, Far);
	auto transform = GetComponent<components::Transform>();
	Vector3 f = transform->GetRotation() * ForwardVector;
	m_data.view = LookAt(transform->GetPosition(), transform->GetPosition() + f, UpVector);
	glCreateBuffers(1, &m_uniformID);
	glNamedBufferStorage(m_uniformID, sizeof(CameraData), &m_data, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformID);
}

void Camera::Update(float deltaTime)
{
	auto transform = GetComponent<components::Transform>();
	if (Inputs::Get().HasAnyPressedKey()) {
		if (Inputs::Get().IsKeyDown('W')) transform->MoveForward(m_speed * deltaTime);
		if (Inputs::Get().IsKeyDown('A')) transform->MoveRight(-m_speed * deltaTime);
		if (Inputs::Get().IsKeyDown('S')) transform->MoveForward(-m_speed * deltaTime);
		if (Inputs::Get().IsKeyDown('D')) transform->MoveRight(m_speed * deltaTime);
		if (Inputs::Get().IsKeyDown('Q')) transform->SetPosition(transform->GetPosition() - UpVector * (m_speed * deltaTime));
		if (Inputs::Get().IsKeyDown('E')) transform->SetPosition(transform->GetPosition() + UpVector * (m_speed * deltaTime));
	}

	Vector2 md = Inputs::Get().GetMouseDelta();
	if (Inputs::Get().IsMouseButtonPressed(EMouseButton::Left) && md != Vector2{ 0.f, 0.f }) {
		m_eulerAngles.yaw -= ToRadians(md.x * m_sensitivity);
		m_eulerAngles.pitch += ToRadians(md.y * m_sensitivity);
		m_eulerAngles.pitch = Clamp(m_eulerAngles.pitch, ToRadians(-89.f), ToRadians(89.f));
		transform->SetRotation(Quaternion(m_eulerAngles));
	}

	if (transform->IsUpdated()) {
		Vector3 f = transform->GetRotation() * ForwardVector;
		m_data.view = LookAt(transform->GetPosition(), transform->GetPosition() + f, UpVector);
		transform->ResetUpdated();
	}
}

void Camera::RefreshBuffer()
{
	glNamedBufferSubData(m_uniformID, 0, sizeof(CameraData), &m_data);
}
