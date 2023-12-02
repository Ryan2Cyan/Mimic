#pragma once
#include <chrono>

namespace MimicUtil
{
	// #############################################################################
	// timer stuct:
	// #############################################################################
	struct Timer
	{
		Timer();
		~Timer(); // scope based: stop is called int he destructor

		void Stop();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _startTimePoint;
	};


	//// #############################################################################
	//// performance counter stuct:
	//// #############################################################################
	//struct PerformanceCounter
	//{
	//	explicit PerformanceCounter();

	//	static std::shared_ptr<PerformanceCounter> Initialise();
	//	void StartPerformanceCounter() noexcept;
	//	const long long EndPerformanceCounter() noexcept;
	//	const long GetFPS() const noexcept;

	//private:
	//	long long _startTime;
	//	long long _elapsedTime;
	//};
}