#ifndef SRN_INPUT_H
#define SRN_INPUT_H

#include <SRN_Platform.h>

#include <Windows.h>

namespace Siren {

	enum SRN_API KeyCodes
	{
		SRN_KEY_A = 0x41,
		SRN_KEY_D = 0x44,
		SRN_KEY_F = 0x46,
		SRN_KEY_S = 0x53,
		SRN_KEY_W = 0x57,
		SRN_KEY_ESC = 0x1B
	};

	struct SRN_API Key
	{
		static int SRN_FORWARD;
		static int SRN_BACKWARD;
		static int SRN_RIGHT;
		static int SRN_LEFT;
		static int SRN_FULLSCREEN_TOGGLE;
		static int SRN_QUIT;
	};

	class SRN_API Input
	{
	public:
		Input();
		~Input();
		static void SetKeyToKeyCode(int keyid, int keyCode);
		static int ProcessInput(LPARAM lParam);
		static void RegisterInputDevices();
		static USHORT GetKeyPressed();
		static LONG GetMouseX();
		static LONG GetMouseY();
		static LONG mouseX;
		static LONG mouseY;

	private:
		static USHORT key;
		static bool pressed;
		
	};

}

#endif // SRN_INPUT_H