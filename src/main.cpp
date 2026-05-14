#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "App.h"
#include "utils/Logger.h"


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND CreateGameWindow(HINSTANCE hInstance, bool isFake, App* app = NULL);


int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	Logger logger{};

	CreateGameWindow(hInstance, true);
	App app{};
	HWND hwnd = CreateGameWindow(hInstance, false, &app);

	HDC hdc = GetDC(hwnd);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	const float dt = 1.f / 60.f;
	const std::chrono::nanoseconds fixedstep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(dt));
	auto previous = std::chrono::steady_clock::now();
	std::chrono::nanoseconds accumulator(0);

	MSG msg{};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			auto current = std::chrono::steady_clock::now();
			auto deltatime = current - previous;
			if (deltatime > std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(0.2f))) {
				deltatime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(0.2f));
			}
			previous = current;
			accumulator += deltatime;
			while (accumulator >= fixedstep) {
				app.Update(dt);
				accumulator -= fixedstep;
			}

			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			app.RenderFrame();

			SwapBuffers(hdc);
		}
	}

	return 0;
}


HWND CreateGameWindow(HINSTANCE hInstance, bool isFake, App* app)
{
	std::string className = "NoLibGame_Class";
	std::string title = "NoLibGame";
	int width = 800;
	int height = 600;
	if (isFake) {
		className += "_FAKE";
		title += "_FAKE";
		width = 1;
		height = 1;
	}

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = isFake ? DefWindowProc : WindowProcedure;
	wc.lpszClassName = className.c_str();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_OWNDC;
	RegisterClassEx(&wc);

	RECT rect = RECT{ 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	HWND hwnd = CreateWindowEx(0, className.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, app);
	if (hwnd == NULL) {
		LLog("Failed to create {} Window!", isFake ? "FAKE" : "REAL");
		throw std::runtime_error("Failed to create window!");
	}

	if (!isFake) {
		ShowWindow(hwnd, SW_SHOW);
	}
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd{ 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormat;
	if (isFake) {
		pixelFormat = ChoosePixelFormat(hdc, &pfd);
	}
	else {
		int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 4,
			0
		};
		float pixelFloatAttribs[] = { 0, 0 };
		UINT numFormats;

		bool success = wglChoosePixelFormatARB(hdc, pixelAttribs, pixelFloatAttribs, 1, &pixelFormat, &numFormats);
		if (!success || numFormats == 0) {
			LLog("PixelFormat not found!");
			throw std::runtime_error("PixelFormat not found!");
		}

		DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);
	}
	SetPixelFormat(hdc, pixelFormat, &pfd);

	HGLRC hrc;
	if (isFake) {
		hrc = wglCreateContext(hdc);
	}
	else {
		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};
		hrc = wglCreateContextAttribsARB(hdc, 0, contextAttribs);
	}
	wglMakeCurrent(hdc, hrc);

	if (isFake) {
		LoadGLFunctions();

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		DestroyWindow(hwnd);
		UnregisterClass(className.c_str(), hInstance);
	}
	else {
		if (app) app->Init(hwnd, width, height);
	}

	return hwnd;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	App* app = nullptr;
	if (msg == WM_NCCREATE) {
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		app = (App*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (App*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	if (app && app->IsInited()) {
		return app->HandleMessage(msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
