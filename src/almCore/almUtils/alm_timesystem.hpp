#ifndef _ALM_TIMESYSTEM_HPP_
#define _ALM_TIMESYSTEM_HPP_

#include "../src/almCore/alm_platform.hpp"
#include "../src/almCore/alm_engsystem.hpp"

namespace alme
{
	class AlmTimeSystem : public AlmEngineSystem
	{
	public:
		static const uint32_t kSamples = 5;
		using time_list = std::vector<std::pair<std::string, uint32_t>>;
	public:
		AlmTimeSystem(AlmostEngine* engine);

		void BeginMeasurement();
		void EndMeasurement();
		void AddPoint(const std::string &name);

		float deltaTime() const;
		const time_list & timePoints() const;

	private:
		uint32_t m_start;
		uint32_t m_timestamps[kSamples];
		time_list m_timePoints;
		time_list m_timePointsDynamic;
	};
}

#endif // !_ALM_TIMESYSTEM_HPP_