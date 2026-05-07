#include "App.h"
#include "assets/Assets.h"
#include "graphics/Camera.h"
#include "graphics/Shaders.h"
#include "graphics/Triangle.h"
#include "graphics/Plane.h"
#include "utils/Logger.h"
#include "utils/Inputs.h"


App::App()
{
	m_assets = std::make_unique<Assets>();
	m_camera = std::make_unique<Camera>();
	m_shaders = std::make_unique<Shaders>();
	m_inputs = std::make_unique<Inputs>();
	m_triangle = std::make_unique<Plane>();
}

App::~App()
{
	m_triangle.reset();
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

	ShaderProgram* program = m_shaders->CreateShaderProgram("default", "shaders/default.vert", "shaders/default.frag");
	m_camera->Init(45.f, (float)m_width / (float)m_height, 0.1f, 1000.f);
	m_triangle->Init();

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
	m_shaders->Use("default");

	m_camera->RefreshBuffer();

	m_triangle->Render();
}

void App::Update(float deltaTime)
{
}
