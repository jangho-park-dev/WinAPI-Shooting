#include "Timer.h"
#include <iostream>

Timer::Timer()
	: m_fDeltaTime(0.f), m_fFps(0.f)
{
	m_llFrequency.QuadPart = 0;
	m_llPrevTime.QuadPart = 0;
	m_llCurrTime.QuadPart = 0;
}

// 타이머 초기화
void Timer::Initialize()
{
	// 주파수 얻기
	QueryPerformanceFrequency(&m_llFrequency);

	// 이전 시간 초기화
	QueryPerformanceCounter(&m_llPrevTime);
}

// 매 프레임마다 호출
void Timer::Tick()
{
	const float TARGET_DELTA = 1.f / 60.f;

	// 목표 프레임 시간이 될 때까지 대기
	do
	{
		QueryPerformanceCounter(&m_llCurrTime);
		LONGLONG deltaCounts = m_llCurrTime.QuadPart - m_llPrevTime.QuadPart;
		m_fDeltaTime = static_cast<float>(deltaCounts) / static_cast<float>(m_llFrequency.QuadPart);
	} while (m_fDeltaTime < TARGET_DELTA);

	// 이전 시간을 현재 시간으로 업데이트
	m_llPrevTime = m_llCurrTime;

	// deltaTime 제한(20fps 정도)
	/*if (m_fDeltaTime > 0.05f)
		m_fDeltaTime = 0.05f;*/

	// FPS 계산
	m_fFps = m_fDeltaTime > 0.f ? 1.f / m_fDeltaTime : 0.f;
}

float Timer::GetDeltaTime() const
{
	return m_fDeltaTime;
}

float Timer::GetFPS() const
{
	return m_fFps;
}

