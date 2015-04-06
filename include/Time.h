#ifndef SRN_TIME_H
#define SRN_TIME_H

#include <SRN_Platform.h>

#include <Windows.h>

namespace Siren {

	static class SRN_API Time
	{
	public:
		Time();

		static float fps;
		static float deltaTime;

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

#endif // SRN_TIME_H