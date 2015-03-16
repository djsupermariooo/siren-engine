#pragma once

#include "GLUtil.h"

class App
{
public:
	App(int width, int height, const char* title);
	virtual ~App();

	// MAIN APPLICATION LOOP
	int Run();

	// FRAMEWORK METHODS
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

protected:
	HWND						m_hWnd;
	HINSTANCE					m_hAppInst;
	HDC							m_hDC;
	HGLRC						m_hRC;
	UINT						m_ClientWidth;
	UINT						m_ClientHeight;
	DWORD						m_WndStyle;
	char*						m_AppTitle;
	float						m_FPS;

	bool InitWindow();

	bool InitGL();

	void CalculateFPS(float dt);

	void Shutdown();
};

