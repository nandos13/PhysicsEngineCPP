#include "stdafx.h"
#include "Time.h"

// Static variable declaration
bool Time::m_paused;
float Time::m_deltaTime;

const bool Time::IsPaused()
{
	return m_paused;
}

void Time::SetPause(const bool state)
{
	m_paused = state;
}

const float Time::DeltaTime()
{
	return m_deltaTime;
}

void Time::SetDeltaTime(const float dt)
{
	m_deltaTime = dt;
}
