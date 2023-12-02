#include "PerformanceCounter.h"
#include <utility/Logger.h>
#include <iostream>

namespace MimicUtil
{

	// #############################################################################
	// timer functions:
	// #############################################################################
	Timer::Timer()
	{
		_startTimePoint = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		Stop();
	}

	void Timer::Stop()
	{
		const auto endTimePoint = std::chrono::high_resolution_clock::now();
		const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch().count();
		const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		const auto duration = end - start;
		const float milliSeconds = duration * 0.001;

		MIMIC_DEBUG_LOG("% [micro], % [milli]", duration, milliSeconds);
	}

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