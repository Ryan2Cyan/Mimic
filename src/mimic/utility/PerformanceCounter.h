#pragma once
#include <memory>

namespace Mimic
{
	// #############################################################################
	// performance counter stuct:
	// #############################################################################
	struct PerformanceCounter
	{
		explicit PerformanceCounter();

		static std::shared_ptr<PerformanceCounter> Initialise();
		void StartPerformanceCounter() noexcept;
		const long long EndPerformanceCounter() noexcept;
		const long GetFPS() const noexcept;

	private:
		long long _startTime;
		long long _elapsedTime;
	};
}