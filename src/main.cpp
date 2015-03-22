#include "App.h"
#include "Input.h"
#include <iostream>
#include <stdint.h>

class TestApp : public App
{
public:
	TestApp(int width, int height, const char* title);
	~TestApp();

	// OVERRIDES
	bool Init() override;
	void Update(float deltaTime) override;
	void Render() override;
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

void TestApp::Update(float deltaTime)
{
	if (Input::GetKeyPressed() == 0x1B)
	{
		TestApp::Quit();
	}
	//std::cout << Time::deltaTime << std::endl;
}

void TestApp::Render()
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glVertex2f( 0.5f,  0.5f);
	glVertex2f( 0.5f, -0.5f);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(-0.5f,  0.5f);
	glEnd();
}

int main()
{
	TestApp app(800, 600, "Siren Engine | Open GL");

	if (!app.Init())
		return 1;
	return app.Run();
}