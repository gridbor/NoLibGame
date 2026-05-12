#include "App.h"
#include "assets/Assets.h"
#include "graphics/Camera.h"
#include "graphics/Shaders.h"
#include "graphics/Triangle.h"
#include "graphics/Plane.h"
#include "utils/Logger.h"
#include "utils/Inputs.h"
#include "graphics/gizmo/CoordinateSystem.h"


App::App()
{
	m_assets = std::make_unique<Assets>();
	m_camera = std::make_unique<Camera>(Vector3(-3.f, 0.f, 1.f), Vector3(ToRadians(0.f), ToRadians(0.f), ToRadians(0.f)));
	m_shaders = std::make_unique<Shaders>();
	m_inputs = std::make_unique<Inputs>();
	m_testObject = std::make_unique<Plane>();
	m_coords = std::make_unique<gizmo::CoordinateSystem>();
}

App::~App()
{
	m_coords.reset();
	m_testObject.reset();
	m_inputs.reset();
	m_shaders.reset();
	m_camera.reset();
	m_assets.reset();
}

void App::Init(HWND hwnd, int width, int height)
{
	if (m_inited) return;
	m_hwnd = hwnd;
	m_width = width;
	m_height = height;

	m_shaders->CreateShaderProgram("default", "shaders/default.vert", "shaders/default.frag");
	m_shaders->CreateShaderProgram("simple", "shaders/color_only.vert", "shaders/color_only.frag");

	m_camera->Init(45.f, (float)m_width / (float)m_height, 0.1f, 1000.f);
	m_gizmoView = LookAt(Vector3(0.f, 0.f, 2.5f), Vector3(), Vector3(0.f, 1.f, 0.f));

	m_testObject->Init();
	m_coords->Init();

	m_inited = true;
}

LRESULT App::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;
	switch (msg) {
	case WM_SIZE:
		m_width = LOWORD(lparam);
		m_height = HIWORD(lparam);
		glViewport(0, 0, m_width, m_height);
		break;
	case WM_KEYDOWN:
		switch (wparam) {
		case VK_ESCAPE:
			DestroyWindow(m_hwnd);
			break;
		default:
			m_inputs->AddKey(wparam);
			break;
		}
		break;
	case WM_KEYUP: m_inputs->RemoveKey(wparam); break;
	case WM_MOUSEMOVE: m_inputs->UpdateMousePosition(LOWORD(lparam), HIWORD(lparam)); break;
	case WM_MOUSEWHEEL:
		break;
	case WM_LBUTTONDOWN: m_inputs->AddMouseButton(EMouseButton::Left); break;
	case WM_MBUTTONDOWN: m_inputs->AddMouseButton(EMouseButton::Middle); break;
	case WM_RBUTTONDOWN: m_inputs->AddMouseButton(EMouseButton::Right); break;
	case WM_LBUTTONUP: m_inputs->RemoveMouseButton(EMouseButton::Left); break;
	case WM_MBUTTONUP: m_inputs->RemoveMouseButton(EMouseButton::Middle); break;
	case WM_RBUTTONUP: m_inputs->RemoveMouseButton(EMouseButton::Right); break;
	default:
		result = DefWindowProc(m_hwnd, msg, wparam, lparam);
	}
	return result;
}

void App::RenderFrame()
{
	m_camera->RefreshBuffer();
	m_shaders->Use("default");
	m_testObject->Render();

	float ratio = (float)m_width / (float)m_height;
	float viewportSize = (float)m_width * 0.2f;
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, (GLsizei)viewportSize, (GLsizei)(viewportSize / ratio));
	CameraData cd{};
	cd.projection = m_camera->GetPerspective();
	Matrix4 viewMatrix = m_camera->GetView();
	for (size_t i = 0; i < 3; i++) viewMatrix.m4[i] = 0.f;
	cd.view = m_gizmoView;
	glNamedBufferSubData(m_camera->GetUniformID(), 0, sizeof(CameraData), &cd);
	m_shaders->Use("simple");
	m_shaders->SetUniformMatrix("uModel", viewMatrix);
	m_coords->Render();

	glViewport(0, 0, m_width, m_height);
}

void App::Update(float deltaTime)
{
	m_camera->Update(deltaTime);
	m_inputs->Update(deltaTime);
}
