#ifndef TIME_H
#define TIME_H

#include <SRN_Platform.h>
#include <Windows.h>

namespace Siren {

	static class SRN_API Time
	{
	public:
		// Constructor
		Time();

		static float fps;
		static float deltaTime;

		//****************************************//
		//	METHODS FOR FPS                       //
		//****************************************//
		static float getFPS();

		static float getSecondsPerCount();
		static float getDeltaTime(float secondsPerCount);
		static void resetTime();
		static void calculateFPS(float deltaTime);

	private:
		static __int64 prevTime;
		static __int64 countsPerSec;
		static __int64 curTime;
		static float secondsPerCount;

		static __int64 getPrevTime();
	};

}

#endif