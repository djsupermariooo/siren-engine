///////////////////////////////////////////////////////////////////////////////////////////////////////
//																									 //
//		Siren Engine																				 //
//		C++ Game Engine																				 //
//		Mario Month																					 //
//																									 //
//																									 //
//		INPUT CLASS																					 //
//																									 //
//		The Input class allows a client to process any input needed for their application. So far	 //
//		this class has support for basic keyboard and controller controls. Only a Xbox360 wired		 //
//		controller has been tested with the controller support. Theoretically, it should work for	 //
//		any. This class attempts to abstract away any low level access needed by the client,		 //
//		provide an easy to use interface to input controls, and allow easy configuration for both	 //
//		the client and the client's end users.														 //
//																									 //
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SRN_INPUT_H
#define SRN_INPUT_H

#include <SRN_Platform.h>

#include <Windows.h>
#include <Xinput.h>

namespace Siren {

	// List of key codes to make identification easier.
	enum SRN_API KeyCodes
	{
		SRN_KEY_A =		0x41,
		SRN_KEY_D =		0x44,
		SRN_KEY_F =		0x46,
		SRN_KEY_S =		0x53,
		SRN_KEY_W =		0x57,
		SRN_KEY_ESC =	0x1B
	};

	// List of "generic" variables that can store the key the client wants to bind to it.
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
		// Constructor & Destructor
		Input();
		~Input();

		// Client called methods
		static void SetKeyToKeyCode(int keyid, int keyCode);
		static int ProcessInput(LPARAM lParam);
		static void RegisterInputDevices();

		static void vibrateController(int leftVal = 0, int rightVal = 0);
		static bool isControllerConnected();
		static XINPUT_STATE getControllerState();
		static void setPlayerNum(int playerNum) { controllerNum = playerNum; }

		// Getters
		static USHORT GetKeyPressed();
		static LONG GetMouseX() { return mouseX; }
		static LONG GetMouseY() { return mouseY; }

		// Fields
		static LONG mouseX;
		static LONG mouseY;

	private:
		static USHORT key;
		static bool pressed;
		static XINPUT_STATE controllerState;
		static int controllerNum;
		
	};

}

#endif // SRN_INPUT_H