#pragma once
#include <memory>

#include "GLFunctions.h"
#include "graphics/RenderData.h"


class Assets;
class Camera;
class Shaders;
class Inputs;
class Triangle;
class Plane;
namespace gizmo { class CoordinateSystem; }
namespace lights { class Light; }

class App {
public:
	App();
	~App();

	void Init(HWND hwnd, int width, int height);
	bool IsInited() const { return m_inited; }
	LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam);
	void RenderFrame();
	void Update(float deltaTime);

private:
	HWND m_hwnd = NULL;
	int m_width = 0;
	int m_height = 0;
	bool m_inited = false;
	std::unique_ptr<Assets> m_assets;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Shaders> m_shaders;
	std::unique_ptr<Inputs> m_inputs;
	std::unique_ptr<Plane> m_testObject;
	Matrix4 m_gizmoView{};
	std::unique_ptr<gizmo::CoordinateSystem> m_coords;
	std::unique_ptr<lights::Light> m_lights;

};
