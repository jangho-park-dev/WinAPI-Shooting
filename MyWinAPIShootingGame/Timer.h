#pragma once
#include <windows.h>

class Timer
{
private:
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llPrevTime;
	LARGE_INTEGER m_llCurrTime;

	float m_fDeltaTime;
	float m_fFps;

public:
	static Timer& GetInstance()
	{
		static Timer instance;
		return instance;
	}

	Timer();

	void Initialize();
	void Tick();

	float GetDeltaTime() const;
	float GetFPS() const;
};

