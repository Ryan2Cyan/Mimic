#pragma once
#include <memory>


namespace MimicEngine
{
	/// <summary>
	/// Returns delta time (difference between the current time between this frame and the last frame).
	/// </summary>
	/// <returns>Delta time for the current frame.</returns>
	float DeltaTime();

	/// <summary>
	/// Handles calculation of delta time and fixed delta time each frame.
	/// </summary>
	struct Environment
	{
		/// <summary>
		/// Get current delta time. Static implementation of this function is required
		/// for global implementation. 
		/// </summary>
		/// <returns>Delta time for the current frame.</returns>
		static float GetDeltaTime() noexcept;

		/// <summary>
		/// Sets the time step inbetween each FixedUpdate() frame. This is set to 0.02 by default.
		/// </summary>
		/// <param name="fixedDeltaTime">New time step inbetween each FixedUpdate() loop.</param>
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