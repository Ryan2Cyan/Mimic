#pragma once
#include <memory>


namespace MimicEngine
{
	float DeltaTime();

	// #############################################################################
	// environment stuct:
	// #############################################################################
	struct Environment
	{

		/// <summary>
		/// Get current delta time. Static implementation of this function is required
		/// for global implementation.
		/// </summary>
		static float GetDeltaTime() noexcept;

	private:
		friend struct MimicCore;

		static std::shared_ptr<Environment> Initialise(const float& idealFramerate = 70.0f);
		static void Func()
		{

		};
		void CalculateDeltaTime();

		static float _deltaTime;
		float _idealFramerate;
		float _lastTimeDelta;
	};
}