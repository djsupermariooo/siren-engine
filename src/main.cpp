#include "App.h"

class TestApp : public App
{
public:
	TestApp(int width, int height, const char* title);
	~TestApp();

	// OVERRIDES
	bool Init() override;
	void Update(float dt) override;
	void Render() override;
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

TestApp::TestApp(int width, int height, const char* title) : App(width, height, title)
{

}

TestApp::~TestApp()
{

}

bool TestApp::Init()
{
	if (!App::Init())
		return false;

	return true;
}

void TestApp::Update(float dt)
{

}

void TestApp::Render()
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

LRESULT TestApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	default:
		return App::MsgProc(hWnd, msg, wParam, lParam);
		break;
	}
}

/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TestApp app(hInstance);

	if (!app.Init())
		return 1;
	return app.Run();
}*/

int main()
{
	TestApp app(800, 600, "Siren Engine | Open GL");

	if (!app.Init())
		return 1;
	return app.Run();
}