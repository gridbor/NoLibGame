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
	m_correction.m[2] = -1.f;
	m_correction.m[4] = -1.f;
	m_correction.m[9] = 1.f;
	m_correction.m[15] = 1.f;
}

Camera::~Camera()
{
	glDeleteBuffers(1, &m_uniformID);
}

void Camera::Init(float fov, float aspect, float Near, float Far)
{
	auto transform = GetComponent<components::Transform>();
	m_data.projection = Perspective(fov, aspect, Near, Far);
	Vector3 f = transform->GetRotation() * ForwardVector;
	Vector3 u = transform->GetRotation() * UpVector;
	m_data.view = LookAt(transform->GetPosition(), transform->GetPosition() + f, u); // m_correction * transform->GetMatrix().Invert(true);
	glCreateBuffers(1, &m_uniformID);
	glNamedBufferStorage(m_uniformID, sizeof(CameraData), &m_data, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformID);
}

void Camera::InputProcess()
{
}

void Camera::Update(float deltaTime)
{
	auto transform = GetComponent<components::Transform>();
	if (Inputs::Get().IsKeyDown('W')) transform->MoveForward(2.f * deltaTime);
	if (Inputs::Get().IsKeyDown('A')) transform->MoveRight(-2.f * deltaTime);
	if (Inputs::Get().IsKeyDown('S')) transform->MoveForward(-2.f * deltaTime);
	if (Inputs::Get().IsKeyDown('D')) transform->MoveRight(2.f * deltaTime);
	if (Inputs::Get().IsKeyDown('Q')) transform->SetPosition(transform->GetPosition() - UpVector * (2.f * deltaTime));
	if (Inputs::Get().IsKeyDown('E')) transform->SetPosition(transform->GetPosition() + UpVector * (2.f * deltaTime));

	if (Inputs::Get().IsMouseButtonPressed(EMouseButton::Left)) {
		Vector2 md = Inputs::Get().GetMouseDelta();
		m_eulerAngles.yaw += ToRadians(-md.x * 10.f * deltaTime);
		m_eulerAngles.pitch += ToRadians(md.y * 10.f * deltaTime);
		m_eulerAngles.pitch = Clamp(m_eulerAngles.pitch, ToRadians(-89.f), ToRadians(89.f));
		transform->SetRotation(Quaternion(m_eulerAngles));
	}
	/*Vector3 f;
	f.x = std::cosf(m_eulerAngles.yaw) * std::cosf(m_eulerAngles.pitch);
	f.y = std::sinf(m_eulerAngles.yaw) * std::cosf(m_eulerAngles.pitch);
	f.z = std::sinf(m_eulerAngles.pitch);
	f.Normalize();
	Vector3 r = Vector3::Normalize(Cross(f, UpVector));
	Vector3 u = Cross(r, f);
	Matrix4 view;
	view.m[0] = r.x; view.m[1] = u.x; view.m[2] = -f.x;
	view.m[4] = r.y; view.m[5] = u.y; view.m[6] = -f.y;
	view.m[8] = r.z; view.m[9] = u.z; view.m[10] = -f.z;
	view.m[12] = -Dot(r, transform->GetPosition());
	view.m[13] = -Dot(u, transform->GetPosition());
	view.m[14] = Dot(f, transform->GetPosition());
	m_data.view = view;*/
	m_data.view = m_correction * transform->GetMatrix().Invert(true);
}

void Camera::RefreshBuffer()
{
	glNamedBufferSubData(m_uniformID, 0, sizeof(CameraData), &m_data);
}
