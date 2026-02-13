#include "Timer.h"

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
	// 현재 시간 얻기
	QueryPerformanceCounter(&m_llCurrTime);

	// 경과 시간 계산
	LONGLONG deltaCounts = m_llCurrTime.QuadPart - m_llPrevTime.QuadPart;
	// 초 단위로 변환
	m_fDeltaTime = static_cast<float>(deltaCounts) / static_cast<float>(m_llFrequency.QuadPart);

	// 이전 시간을 현재 시간으로 업데이트
	m_llPrevTime = m_llCurrTime;

	// deltaTime 제한(fps가 너무 낮아지는 경우 방지)
	if (m_fDeltaTime > 0.05f)
		m_fDeltaTime = 0.05f;

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

