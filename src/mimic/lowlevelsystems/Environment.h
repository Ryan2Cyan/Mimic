#pragma once
#include <memory>


namespace Mimic
{
	float DeltaTime();

	// #############################################################################
	// environment stuct:
	// #############################################################################
	struct Environment
	{
		explicit Environment(const float idealTime);

		static float GetDeltaTime();
		void CalculateDeltaTime();
	private:
		friend struct MimicCore;

		std::shared_ptr<Environment> Initialise(const float idealFramerate);
		static float _deltaTime;
		float _lastTime;
		const float _idealFramerate;
	};
}