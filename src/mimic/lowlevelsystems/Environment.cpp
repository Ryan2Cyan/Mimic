#include "Environment.h"
#include <SDL/SDL.h>

namespace Mimic
{
	// global: access deltaTime from anywhere.
	float Environment::_deltaTime;
	float DeltaTime()
	{
		return Environment::GetDeltaTime();
	}

	Environment::Environment(const float idealFramerate) : _lastTime(0.0f), _idealFramerate(idealFramerate) 
	{

	}

	std::shared_ptr<Environment> Environment::Initialise(const float idealFramerate)
	{
		return std::make_shared<Environment>(idealFramerate);
	}

	// access private static member _deltaTime:
	float Environment::GetDeltaTime()
	{
		return _deltaTime;
	}

	void Environment::CalculateDeltaTime()
	{
		const float time = SDL_GetTicks();
		const float difference = time - _lastTime;
		const float deltaTime = difference / 1000.0f;
		_lastTime = time;

		// delay program to prevent CPU from doing unnecessary calculations:
		const float idealTime = 1.0f / _idealFramerate;
		if (idealTime > deltaTime) SDL_Delay((idealTime - deltaTime) * 1000.0f);
		Environment::_deltaTime = deltaTime;
	}
}