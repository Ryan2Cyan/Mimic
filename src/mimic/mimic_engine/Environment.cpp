#include "Environment.h"
#include <mimic_utility/Logger.h>

#include <SDL/SDL.h>

namespace MimicEngine
{
	// Global Access: Get current delta time value.
	float Environment::_deltaTime;
	float DeltaTime()
	{
		return Environment::GetDeltaTime();
	}

	void Environment::SetFixedDeltaTime(const float& fixedDeltaTime)
	{
		_fixedDeltaTime = fixedDeltaTime;
	}

	std::shared_ptr<Environment> Environment::Initialise(const float& idealFramerate)
	{
		std::shared_ptr<Environment> environment = std::make_shared<Environment>();
		environment->_idealFramerate = idealFramerate;
		return environment;
	}

	float Environment::GetDeltaTime() noexcept
	{
		return _deltaTime;
	}

	void Environment::CalculateDeltaTime()
	{
		const auto time = SDL_GetTicks();
		const auto difference = time - _lastTimeDelta;
		const auto deltaTime = difference / 1000.0f;
		_lastTimeDelta = time;

		// Delay program to prevent CPU from doing extra unnecessary calculations:
		const float idealTime = 1.0f / _idealFramerate;
		if (idealTime > deltaTime) SDL_Delay((idealTime - deltaTime) * 1000.0f);
		Environment::_deltaTime = deltaTime;
	}

	bool Environment::FixedTimeTick()
	{
		const auto time = SDL_GetTicks();
		const auto difference = time - _lastTimeDelta;
		if (difference > _fixedDeltaTime)
		{
			_lastTimeFixedDelta = time;
			return true;
		}
		return false;
	}
}