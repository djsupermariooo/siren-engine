#include "App.h"

using namespace Siren;

namespace
{
	App* g_pApp = NULL;
}

LRESULT CALLBACK App::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
	//m_hWnd_Child = NULL;
	m_hDC = NULL;
	m_hRC = NULL;
	m_ClientWidth = width;
	m_ClientHeight = height;
	m_AppTitle = (LPSTR)title;
	m_WndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	g_pApp = this;
}

App::~App()
{
}

// MAIN APPLICATION LOOP
int App::Run()
{
	float secPerCount = Time::getSecondsPerCount();

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
			float deltaTime = Time::getDeltaTime(secPerCount);

			// UPDATE
			Update(deltaTime);
			// RENDER
			Render();
			// SWAP BUFFERS
			SwapBuffers(m_hDC);

			// RESET PREVIOUS TIME FOR NEXT FRAME
			Time::resetTime();
			
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
	wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
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

	Input::RegisterInputDevices();

	return true;
}

bool App::InitGL(HWND m_hWnd_Child) //REMOVE HWND m_hWnd_Child
{
	// CREATE DEVICE CONTEXT
	m_hDC = GetDC(m_hWnd_Child); //CHANGE TO m_hWnd

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

	// INITIALIZE GLEW
	if (glewInit())
		return OutErrorMsg("Failed to initialize glew.");

	return true;
}

bool App::Init()
{
	//if (!InitWindow()) return false; //UNCOMMENT
	//if (!InitGL()) return false; //UNCOMMENT

	return true;
}

LRESULT App::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_INPUT:
		Input::ProcessInput(lParam);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void App::Shutdown()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);
	ReleaseDC(m_hWnd_Child, m_hDC); //CHANGE TO m_hWnd
}

void Siren::App::Quit()
{
	PostQuitMessage(0);
}

UINT Siren::App::getWidth()
{
	return m_ClientWidth;
}

UINT Siren::App::getHeight()
{
	return m_ClientHeight;
}
