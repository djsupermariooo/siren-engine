#include "App.h"

#include <iostream>

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

App::App(GLint width, GLint height, const char* title, bool fullscreen)
{
	m_hAppInst = GetModuleHandle(NULL);
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hRC = NULL;
	m_ClientWidth = width;
	m_ClientHeight = height;
	m_AppTitle = (LPSTR)title;
	m_FullScreen = fullscreen;
	g_pApp = this;
}

App::~App()
{
}

// MAIN APPLICATION LOOP
GLint App::Run()
{
	GLfloat secPerCount = Time::getSecondsPerCount();

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
			GLfloat deltaTime = Time::getDeltaTime(secPerCount);

			// UPDATE
			Update(deltaTime);
			// RENDER
			Render();

			// CALCULATE FPS
			Time::calculateFPS(deltaTime);

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

	if (m_FullScreen)														// Attempt fullscreen mode?
	{
		DEVMODE dmScreenSettings;											// Device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));				// Make sure memory is cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);					// Size of the devmode structure
		dmScreenSettings.dmPelsWidth = m_ClientWidth;						// Selected screen width
		dmScreenSettings.dmPelsHeight = m_ClientHeight;						// Selected screen height
		dmScreenSettings.dmBitsPerPel = 32;									// Selected bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try to set selected mode and get results. Note: CDS_FULLSCREEN gets rid of start bar
		std::cout << ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) << std::endl;
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If the mode fails, offer two options. Quit or run in a window
			if (MessageBox(NULL, "The requested fullscreen mode is not supported by \nyour video card. Use windowed mode instead?",
				"MM GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				m_FullScreen = FALSE;											// Select windowed mode
			}
			else
			{
				// Pop up a message box letting user know the program is closing
				MessageBox(NULL, "Program will now close.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return FALSE;												// Exit and return false
			}
		}
	}

	if (m_FullScreen)
	{
		m_WndStyle = WS_POPUP | WS_VISIBLE;
		ShowCursor(FALSE);
	}
	else
	{
		m_WndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	}

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

	// INITIALIZE GLEW
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
	ReleaseDC(m_hWnd, m_hDC);
}

void App::Quit()
{
	PostQuitMessage(0);
}

UINT App::getWidth()
{
	return m_ClientWidth;
}

UINT App::getHeight()
{
	return m_ClientHeight;
}
