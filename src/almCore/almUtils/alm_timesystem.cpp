#include "alm_timesystem.hpp"
#include <chrono>

namespace alme
{
	inline uint32_t timeNow(){
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	}
}

using namespace alme;

AlmTimeSystem::AlmTimeSystem(AlmostEngine* engine)
	: AlmEngineSystem(engine)
	, m_start(0)
{
	memset(m_timestamps, 0, sizeof(uint32_t) * kSamples);
}

void AlmTimeSystem::BeginMeasurement()
{
	m_start = timeNow();
	m_timePointsDynamic.clear();
}

void AlmTimeSystem::EndMeasurement()
{
	for (uint32_t i(kSamples - 1); i != 0; --i)
		m_timestamps[i] = m_timestamps[i - 1];
	m_timestamps[0] = timeNow() - m_start;
	m_timePoints = m_timePointsDynamic;
}

void AlmTimeSystem::AddPoint(const std::string& name)
{
	m_timePointsDynamic.emplace_back(name, timeNow() - m_start);
}

float AlmTimeSystem::deltaTime() const
{
	uint32_t sum = 0;
	for (uint32_t i(0); i < kSamples; ++i)
		sum += m_timestamps[i];
	return ((static_cast<float>(sum) / static_cast<float>(kSamples)) / 1000.0f);
}

const AlmTimeSystem::time_list & AlmTimeSystem::timePoints() const
{
	return m_timePoints;
}
