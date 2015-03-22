#include "Input.h"
#include "SRN_Platform.h"
#include <iostream>

USHORT Input::key;

Input::Input()
{
}

Input::~Input()
{
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
			Input::key = raw->data.keyboard.VKey;
			std::cout << Input::key << std::endl;
		}
	}
	delete[] lpb;
	return 0;

#elif defined(SRN_OS_LINUX)

#elif defined(SRN_OS_MAC)

#endif

}

USHORT Input::GetKeyPressed()
{
	USHORT keyPressed = Input::key;
	Input::key = 0;
	return keyPressed;
}

#ifdef SRN_OS_WINDOWS
void Input::RegisterInputDevices()
{
	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x06;
	Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[0].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE) {
		std::cout << "Registration failed." << std::endl;
	}
}
#endif