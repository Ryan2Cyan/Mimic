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

		static const float GetDeltaTime() noexcept;
		void CalculateDeltaTime();

	private:
		friend struct MimicCore;

		std::shared_ptr<Environment> Initialise(const float& idealFramerate);
		const float _idealFramerate;
		static float _deltaTime;
		float _lastTimeDelta;
	};
}