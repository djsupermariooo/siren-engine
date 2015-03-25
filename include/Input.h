#ifndef SRN_INPUT_H
#define SRN_INPUT_H

#include <SRN_Platform.h>
#include <Windows.h>

namespace Siren {

	class SRN_API Input
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

}

#endif