#include "stdafx.h"
#include "Timer.h"

Timer::Timer(float delay)
	: m_Time{ 0.f }
	, m_Delay{ delay }
	, m_IsDone{ false }
{

}

Timer::~Timer()
{

}

void Timer::Reset()
{
	m_Time = 0;
	m_IsDone = false;
}

void Timer::Update(float deltaT)
{
	if (m_Time >= m_Delay)
	{
		m_IsDone = true;
	}

	if (!m_IsDone)
	{
		m_Time += deltaT;
	}
}

float Timer::GetTime() const
{
	return m_Time;
}

bool Timer::IsDone() const
{
	return m_IsDone;
}

void Timer::SetDelay(float delay)
{
	m_Delay = delay;
}