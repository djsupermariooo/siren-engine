#include <Windows.h>

class Input
{
public:
	Input();
	~Input();
	static int ProcessInput(LPARAM lParam);
	static void RegisterInputDevices();
	static USHORT GetKeyPressed();

private:
	static USHORT key;
};