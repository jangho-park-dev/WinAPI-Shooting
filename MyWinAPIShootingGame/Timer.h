#pragma once
#include <windows.h>

class Timer
{
public:
	Timer();

	void Initialize();
	void Tick();

	float GetDeltaTime() const;
	float GetFPS() const;

private:
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llPrevTime;
	LARGE_INTEGER m_llCurrTime;

	float m_fDeltaTime;
	float m_fFps;
};

