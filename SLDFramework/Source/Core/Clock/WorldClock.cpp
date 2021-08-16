#include "WorldClock.h"

constexpr float FixedDeltaTime{ 1.0f / 30.0f };

SLD::WorldClock::WorldClock()
	: m_EndTimePoint()
	, m_CurrentTimePoint()
	, m_DeltaTime()
{
}

void SLD::WorldClock::StartTime()
{
	m_CurrentTimePoint = std::chrono::high_resolution_clock::now();

	// https://stackoverflow.com/questions/34141522/c-incorrect-fps-and-deltatime-measuring-using-stdchrono
	using ms = std::chrono::duration<float, std::milli>;
	using seconds = std::chrono::duration<float>;
	m_DeltaTime = std::chrono::duration_cast<seconds>(m_CurrentTimePoint - m_EndTimePoint).count();
}

void SLD::WorldClock::EndTime()
{
	m_EndTimePoint = m_CurrentTimePoint;
}

float SLD::WorldClock::GetFixedDeltaTime() const
{
	return FixedDeltaTime;
}

float SLD::WorldClock::GetDeltaTime() const
{
	return m_DeltaTime;
}
