#ifndef SRN_APP_H
#define SRN_APP_H

#include <GLUtil.h>
#include <Input.h>
#include <SRN_Platform.h>
#include <Time.h>

namespace Siren {

	class SRN_API App
	{
	public:
		// Constructor & Destructor
		App(int width, int height, const char* title, bool fullscreen);
		virtual ~App();

		// Client called methods
		int Run();
		void Quit();
		virtual bool Init();

		// Message Handler
		LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT MainWndProc(HWND, UINT, WPARAM, LPARAM);

		// Interface Methods
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		// Getters & Setters
		void setTitle(char* title);
		char* getTitle();
		void setWidth(UINT width);
		UINT getWidth();
		void setHeight(UINT height);
		UINT getHeight();

	protected:
		// Fields
		HWND						m_hWnd;
		HINSTANCE					m_hAppInst;
		HDC							m_hDC;
		HGLRC						m_hRC;
		UINT						m_ClientWidth;
		UINT						m_ClientHeight;
		DWORD						m_WndStyle;
		char*						m_AppTitle;
		bool						m_FullScreen;
		DEVMODE						dmScreenSettings;

		bool InitWindow();
		bool InitGL();
		void Shutdown();
	};

}

#endif // SRN_APP_H