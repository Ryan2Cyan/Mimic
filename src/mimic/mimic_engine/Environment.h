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

		/// <summary> Get current delta time. Static implementation of this function is required
		/// for global implementation. </summary>
		static float GetDeltaTime() noexcept;

		void SetFixedDeltaTime(const float& fixedDeltaTime);

	private:
		friend struct MimicCore;

		static std::shared_ptr<Environment> Initialise(const float& idealFramerate = 70.0f);
		void CalculateDeltaTime();
		bool FixedTimeTick();

		static float _deltaTime;
		float _fixedDeltaTime = 0.02f; // Unity's default
		float _idealFramerate;
		float _lastTimeDelta;
		float _lastTimeFixedDelta;
	};
}