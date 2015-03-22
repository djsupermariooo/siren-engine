#pragma once

#include "GLUtil.h"
#include "Time.h"

class App
{
public:
	App(int width, int height, const char* title);
	virtual ~App();
	int Run();
	void Quit();
	virtual bool Init();
	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

private:
	void UpdateWindow();

protected:
	HWND						m_hWnd;
	HINSTANCE					m_hAppInst;
	HDC							m_hDC;
	HGLRC						m_hRC;
	UINT						m_ClientWidth;
	UINT						m_ClientHeight;
	DWORD						m_WndStyle;
	char*						m_AppTitle;

	bool InitWindow();
	bool InitGL();
	void Shutdown();
};

