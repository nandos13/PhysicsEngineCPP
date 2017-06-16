#pragma once
class Time
{
private:
	static bool m_paused;
	static float m_deltaTime;
public:
	static const bool IsPaused();
	static void SetPause(const bool state);

	static const float DeltaTime();
	static void SetDeltaTime(const float dt);
};

