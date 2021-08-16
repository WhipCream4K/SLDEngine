
#ifndef SLD_FRAMEWORK_WORLDCLOCK
#define SLD_FRAMEWORK_WORLDCLOCK

#include "../Base.h"

namespace SLD
{
	class WorldClock final
	{
	public:

		WorldClock();
		
		void StartTime();
		void EndTime();
		float GetFixedDeltaTime() const;
		[[nodiscard]] float GetDeltaTime() const;
	
	private:

		std::chrono::high_resolution_clock::time_point m_EndTimePoint;
		std::chrono::high_resolution_clock::time_point m_CurrentTimePoint;
		float m_DeltaTime;
	};
	
}


#endif
