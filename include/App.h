#ifndef SRN_APP_H
#define SRN_APP_H

#include <SRN_Platform.h>
#include <GLUtil.h>
#include <Time.h>
#include <Input.h>

namespace Siren {

	class SRN_API App
	{
	public:
		// Constructor & Destructor
		App(int width, int height, const char* title);
		virtual ~App();

		// Client called methods
		int Run();
		void Quit();
		virtual bool Init();

		// Message Handler
		virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT MainWndProc(HWND, UINT, WPARAM, LPARAM); //DELETE THIS

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

	private:
		// Delete this method before release
		//void UpdateWindow();

	protected:
		HWND						m_hWnd;
		HWND						m_hWnd_Child; //DELETE THIS
		HINSTANCE					m_hAppInst;
		HDC							m_hDC;
		HGLRC						m_hRC;
		UINT						m_ClientWidth;
		UINT						m_ClientHeight;
		DWORD						m_WndStyle;
		char*						m_AppTitle;
		bool						displayPyramid = false;
		bool						displayCube = false;

		bool InitWindow();
		bool InitGL(HWND); //REMOVE HWND
		void Shutdown();
	};

}

#endif