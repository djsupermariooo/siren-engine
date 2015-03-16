#include "App.h"

namespace
{
	App* g_pApp = NULL;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)
		return g_pApp->MsgProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

App::App(int width, int height, const char* title)
{
	m_hAppInst = GetModuleHandle(NULL);
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hRC = NULL;
	m_ClientWidth = width;
	m_ClientHeight = height;
	m_AppTitle = (LPSTR)title;
	m_WndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	m_FPS = 0.0f;
	g_pApp = this;
}

App::~App()
{
}

// MAIN APPLICATION LOOP
int App::Run()
{
	// CALCULATE TIMING
	__int64 prevTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);

	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	float secondsPerCount = 1.0f / countsPerSec;

	// MAIN MESSAGE LOOP
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// CALCULATE DELTA TIME
			__int64 curTime = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
			float deltaTime = (curTime - prevTime) * secondsPerCount;

			// UPDATE
			Update(deltaTime);
			// RENDER
			Render();
			// CALCULATE FPS
			CalculateFPS(deltaTime);
			// SWAP BUFFERS
			SwapBuffers(m_hDC);

			// RESET PREVIOUS TIME FOR NEXT FRAME
			prevTime = curTime;
			
		}
	}
	Shutdown();
	return static_cast<int>(msg.wParam);
}

bool App::InitWindow()
{
	// WNDCLASSEX STRUCTURE
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = MainWndProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = "APPWNDCLASS";
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) return OutErrorMsg("Could not register the window class.");

	// ADJUST WINDOW RECT TO REQUESTED CLIENT SIZE
	RECT rect = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&rect, m_WndStyle, FALSE);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	// CREATE WINDOW
	m_hWnd = CreateWindow("APPWNDCLASS", m_AppTitle, m_WndStyle, x, y, width, height, NULL, NULL, m_hAppInst, NULL);
	if (!m_hWnd) return OutErrorMsg("Failed to create window.");

	// SHOW WINDOW
	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}

bool App::InitGL()
{
	// CREATE DEVICE CONTEXT
	m_hDC = GetDC(m_hWnd);

	// CREATE A PIXELFORMATDESCRIPTOR
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
		return OutErrorMsg("Failed to set pixel format.");

	// CREATE RENDER CONTEXT
	m_hRC = wglCreateContext(m_hDC);
	if (!wglMakeCurrent(m_hDC, m_hRC))
		return OutErrorMsg("Failed to create and activate render context");

	// INITIALIZE GLU
	if (glewInit())
		return OutErrorMsg("Failed to initialize glew.");

	return true;
}

bool App::Init()
{
	if (!InitWindow()) return false;
	if (!InitGL()) return false;


	
	return true;
}

LRESULT App::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void App::CalculateFPS(float dt)
{
	static float elapsed = 0;
	static int frameCnt = 0;

	elapsed += dt;
	frameCnt++;
	if (elapsed >= 1.0f)
	{
		m_FPS = (float)frameCnt;

		// OUTPUT TO WINDOW TITLE
		std::stringstream ss;
		ss << m_AppTitle << " v" << glGetString(GL_VERSION) << " | FPS: " << m_FPS << " | Renderer: " << glGetString(GL_RENDERER);
		SetWindowText(m_hWnd, ss.str().c_str());

		elapsed = 0.0f;
		frameCnt = 0;
	}
}

void App::Shutdown()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);
	ReleaseDC(m_hWnd, m_hDC);
}