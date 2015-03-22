//////////////////////////////////////////////////////////////
//	SIREN ENGINE INCLUDES									//
//////////////////////////////////////////////////////////////

#include "SRN_Platform.h"
#include "Time.h"
#include "App.h"

//////////////////////////////////////////////////////////////
//	SYSTEM INCLUDES											//
//////////////////////////////////////////////////////////////

#include <iostream>
#include <stdint.h>

//////////////////////////////////////////////////////////////
//	STATIC VARIABLES										//
//////////////////////////////////////////////////////////////

__int64 Time::countsPerSec;
__int64 Time::curTime;
__int64 Time::prevTime;
float Time::deltaTime;
float Time::secondsPerCount;
float Time::fps;

// CALCULATE FPS
void Time::calculateFPS(float deltaTime)
{
	static float elapsed = 0;
	static float frameCnt = 0;

	elapsed += deltaTime;
	frameCnt++;
	if (elapsed >= 1.0f)
	{
		fps = (float)frameCnt;

		elapsed = 0.0f;
		frameCnt = 0;
	}
}

float Time::getFPS()
{
	return fps;
}

float Time::getSecondsPerCount()
{
	prevTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);

	countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount = 1.0f / countsPerSec;
	return secondsPerCount;
}

float Time::getDeltaTime(float secondsPerCount)
{
	curTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	deltaTime = (curTime - getPrevTime()) * secondsPerCount;
	return deltaTime;
}

void Time::resetTime()
{
	prevTime = curTime;
}

__int64 Time::getPrevTime()
{
	return prevTime;
}

