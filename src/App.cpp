//////////////////////////////////////////////////////////////
//	SIREN ENGINE											//
//	Copyright 2015 Mario Month								//
//															//
//	APP.CPP													//
//	This class runs the main application loop for the		//
//	engine. It initializes the window, OpenGL, and			//
//	registers for input. It also processes the messages		//
//	provided by the OS.										//
//////////////////////////////////////////////////////////////

#include "App.h"
#include "Input.h"
#include "SRN_Platform.h"
#include <iostream>

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


//////////////////////////////////////////////////////////////
//	CLASS CONSTRUCTOR										//
//															//
//	@PARAM: int width - the width of the window				//
//	@PARAM: int height - the height of the window			//
//	@PARAM: const char* title - the title of the window		//
//////////////////////////////////////////////////////////////

App::App(int width, int height, const char* title)
{
#ifdef SRN_OS_WINDOWS
	m_hAppInst = GetModuleHandle(NULL);
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hRC = NULL;
	m_ClientWidth = width;
	m_ClientHeight = height;
	m_AppTitle = (LPSTR)title;
	m_WndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	g_pApp = this;

	// REGISTER INPUT DEVICES
	Input::RegisterInputDevices();
#endif
}

//////////////////////////////////////////////////////////////
//	CLASS DESTRUCTOR										//
//															//
//	TODO: Implement default destructor (if needed)			//
//////////////////////////////////////////////////////////////

App::~App()
{
}

//////////////////////////////////////////////////////////////
//	MAIN APPLICATION LOOP									//
//															//
//	This method should be called by the client. It			//
//	checks for messages and then calls the update and		//
//	methods that are created by the user.					//
//															//
//	@RETURN - 0 when terminated								//
//////////////////////////////////////////////////////////////

int App::Run()
{
#ifdef SRN_OS_WINDOWS

	float secspercount = Time::getSecondsPerCount();

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
			float deltaTime = Time::getDeltaTime(secspercount);

			Update(deltaTime);
			Render();
			Time::calculateFPS(deltaTime);
			UpdateWindow();
			SwapBuffers(m_hDC);	

			Time::resetTime();
		}
	}
	Shutdown();
	return static_cast<int>(msg.wParam);
#endif
}

//////////////////////////////////////////////////////////////
//	INITIALIZE WINDOW METHOD								//
//															//
//	This method provides the necessary parameters to		//
//	Windows OS for initializing a WIN32 window. It also		//
//	handles registering the window class, adjusting the		//
//	window to the client's specified size, creating the		//
//	window, and then displaying it.							//
//															//
//	@RETURN - true when completed							//
//////////////////////////////////////////////////////////////

bool App::InitWindow()
{
#ifdef SRN_OS_WINDOWS
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

	m_hWnd = CreateWindow("APPWNDCLASS", m_AppTitle, m_WndStyle, x, y, width, height, NULL, NULL, m_hAppInst, NULL);
	if (!m_hWnd) return OutErrorMsg("Failed to create window.");

	ShowWindow(m_hWnd, SW_SHOW);
#endif

	return true;
}

//////////////////////////////////////////////////////////////
//	INITIALIZE OPENGL METHOD								//
//															//
//	This method initializes OpenGL by specifying the		//
//	necessary parameters, creating a device context and		//
//	rendering context, and initializing GLEW.				//
//															//
//	@RETURN - true when completed							//
//////////////////////////////////////////////////////////////

bool App::InitGL()
{
#ifdef SRN_OS_WINDOWS
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
#endif

	// INITIALIZE GLEW
	if (glewInit())
		return OutErrorMsg("Failed to initialize glew.");

	return true;
}

//////////////////////////////////////////////////////////////
//	INITIALIZATION METHOD									//
//															//
//	This method calls the InitWindow and InitGL methods		//
//	and can be overriden by the client.						//
//															//
//	@RETURN - true when completed							//
//////////////////////////////////////////////////////////////

bool App::Init()
{
	if (!InitWindow()) return false;
	if (!InitGL()) return false;

	return true;
}

//////////////////////////////////////////////////////////////
//	MAIN MESSAGE PROCESSOR									//
//															//
//	This method checks the messages from Windows and		//
//	calls the correct methods depending on the type of		//
//	message.												//
//////////////////////////////////////////////////////////////

#ifdef SRN_OS_WINDOWS
LRESULT App::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_INPUT:
		return Input::ProcessInput(lParam);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
#endif

//////////////////////////////////////////////////////////////
//	SHUTDOWN METHOD											//
//															//
//	This method properly shutdowns OpenGL.					//
//////////////////////////////////////////////////////////////

void App::Shutdown()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);
	ReleaseDC(m_hWnd, m_hDC);
}

//////////////////////////////////////////////////////////////
//	UPDATE WINDOW METHOD (PRIVATE)							//
//															//
//	This method is a helper function to update the window	//
//	title based on the version of OpenGL, the FPS, and		//
//	the client's renderer.									//
//////////////////////////////////////////////////////////////

void App::UpdateWindow()
{
#ifdef SRN_OS_WINDOWS
	std::stringstream ss;
	ss << m_AppTitle << " v" << glGetString(GL_VERSION) << " | FPS: " << Time::getFPS() << " | Renderer: " << glGetString(GL_RENDERER);
	SetWindowText(m_hWnd, ss.str().c_str());
#endif
}

//////////////////////////////////////////////////////////////
//	QUIT METHOD												//
//															//
//	This method is called by a client to immediately		//
//	quit the application.									//
//////////////////////////////////////////////////////////////

void App::Quit()
{
#ifdef SRN_OS_WINDOWS
	PostQuitMessage(0);
#endif
}