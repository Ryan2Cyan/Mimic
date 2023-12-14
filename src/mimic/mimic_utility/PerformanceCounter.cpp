#include "PerformanceCounter.h"

namespace MimicUtility
{

	// #############################################################################
	// timer functions:
	// #############################################################################
	

	//// #############################################################################
	//// performance counter stuct:
	//// #############################################################################
	//PerformanceCounter::PerformanceCounter() : _startTime(0.0f), _elapsedTime(0.0f) {}

	//std::shared_ptr<PerformanceCounter> PerformanceCounter::Initialise()
	//{
	//	return std::make_shared<PerformanceCounter>();
	//}

	//void PerformanceCounter::StartPerformanceCounter() noexcept
	//{
	//	_startTime = SDL_GetPerformanceCounter();
	//}

	//const long long PerformanceCounter::EndPerformanceCounter() noexcept
	//{
	//	const long long endTime = SDL_GetPerformanceCounter();
	//	const long long elapsed = endTime - _startTime;
	//	_elapsedTime = elapsed;
	//	return elapsed;
	//}

	//const long PerformanceCounter::GetFPS() const noexcept
	//{
	//	long performanceCountFrequency = SDL_GetPerformanceFrequency();
	//	long elapsedTimeLong = (long)_elapsedTime;
	//	long millisecondsPerFrame = (1000.0f * elapsedTimeLong) / performanceCountFrequency;
	//	return performanceCountFrequency / elapsedTimeLong;
	//}
}