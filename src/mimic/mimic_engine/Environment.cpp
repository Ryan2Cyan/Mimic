#include "Environment.h"

#include <SDL/SDL.h>

namespace MimicEngine
{
	// Global Access: Get current delta time value.
	float Environment::_deltaTime;
	float DeltaTime()
	{
		return Environment::GetDeltaTime();
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
		const float time = SDL_GetTicks();
		const float difference = time - _lastTimeDelta;
		const float deltaTime = difference / 1000.0f;
		_lastTimeDelta = time;

		// delay program to prevent CPU from doing unnecessary calculations:
		const float idealTime = 1.0f / _idealFramerate;
		if (idealTime > deltaTime) SDL_Delay((idealTime - deltaTime) * 1000.0f);
		Environment::_deltaTime = deltaTime;
	}
}