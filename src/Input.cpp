//////////////////////////////////////////////////////////////////////////////////////////////////////
//		INPUT CLASS																					//
//////////////////////////////////////////////////////////////////////////////////////////////////////	

#include "Input.h"
#include "SRN_Platform.h"
#include <iostream>

using namespace Siren;

////////////////////
//		STATIC VARIABLES
///////////////////

USHORT Input::key;
bool Input::pressed;
LONG Input::mouseX;
LONG Input::mouseY;
XINPUT_STATE Input::controllerState;
int Input::controllerNum;

int Key::SRN_FORWARD;
int Key::SRN_BACKWARD;
int Key::SRN_RIGHT;
int Key::SRN_LEFT;
int Key::SRN_FULLSCREEN_TOGGLE;
int Key::SRN_QUIT;

///////////////////
//		CONSTRUCTOR
//////////////////

Input::Input()
{
}

//////////////////
//		DESTRUCTOR
/////////////////

Input::~Input()
{
}
	
void Input::SetKeyToKeyCode(int keyid, int keyCode)
{
	keyid = keyCode;
}

int Input::ProcessInput(LPARAM lParam)
{
#ifdef SRN_OS_WINDOWS
	UINT dwSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
		return 0;

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		if (raw->data.keyboard.Flags == RI_KEY_MAKE)
		{
			pressed = true;
			Input::key = raw->data.keyboard.VKey;
		}
		else if (raw->data.keyboard.Flags == RI_KEY_BREAK)
		{
			pressed = false;
			Input::key = 0;
		}
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		Input::mouseX = raw->data.mouse.lLastX;
		Input::mouseY = raw->data.mouse.lLastY;
	}
	delete[] lpb;
	return 0;

#elif defined(SRN_OS_LINUX)

#elif defined(SRN_OS_MAC)

#endif // SRN_OS

}

USHORT Input::GetKeyPressed()
{
	USHORT keyPressed = Input::key;
	return keyPressed;
}

#ifdef SRN_OS_WINDOWS
void Input::RegisterInputDevices()
{
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x06;
	Rid[0].dwFlags = RIDEV_NOLEGACY;
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x02;
	Rid[1].dwFlags = RIDEV_NOLEGACY;
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
		std::cout << "Registration failed." << std::endl;
	}
}

void Input::vibrateController(int leftVal, int rightVal)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftVal;
	vibration.wRightMotorSpeed = rightVal;

	XInputSetState(controllerNum, &vibration);
}

bool Input::isControllerConnected()
{
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	DWORD Result = XInputGetState(controllerNum, &controllerState);
	if (Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

XINPUT_STATE Input::getControllerState()
{
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	XInputGetState(controllerNum, &controllerState);

	return controllerState;
}

#endif // SRN_OS