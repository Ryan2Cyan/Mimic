#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace MimicEngine
{
	struct MimicCore;
	struct Camera;

	/// <summary>
	/// Contains a origin and direction. Exclusively used by the InputHandler struct to project
	/// rays from the camera to the user's mouse screen-space position.
	/// </summary>
	struct Ray
	{
		Ray() : Origin(glm::vec3(0.0f)), Direction(glm::vec3(0.0f)) {}
		Ray(const glm::vec3& ori, const glm::vec3& dir) : Origin(ori), Direction(dir) {}
		glm::vec3 Origin;
		glm::vec3 Direction;
	};

	/// <summary>
	/// Handles all user inputs (e.g. mouse down, keys pressed, and clicking on elements of the screen).
	/// </summary>
	struct InputHandler
	{
		/// <summary>
		/// Return true if key is pressed or held, otherwise false.
		/// </summary>
		/// <param name="key">SDL virtual key representation (e.g. SDK_a).</param>
		/// <returns>True if the key is held, otherwise false.</returns>
		bool IsKey(const int& key);

		/// <summary>
		/// Return true if key is pressed, otherwise false. 
		/// </summary>
		/// <param name="key">SDL virtual key representation (e.g. SDK_a).</param>
		/// <returns>True if the key was pressed this frame, false otherwise.</returns>
		bool IsKeyPressed(const int& key);

		/// <summary>
		/// Return true if key was released this frame, otherwise false.
		/// </summary>
		/// <param name="key">SDL virtual key representation (e.g. SDK_a).</param>
		/// <returns>True if the key has been released this frame, false otherwise.</returns>
		bool IsKeyReleased(const int& key);

		glm::ivec2 GetCursorPosition() const;

	private:
		friend struct MimicCore;

		static std::shared_ptr<InputHandler> Initialise(bool& applicationRunning);

		void Update();
		void ClearTemp();

		Ray MousePositionProject(const std::shared_ptr<Camera>& cam, const glm::ivec2& cursorPos) const;

		std::list<int> _keys;
		std::vector<int> _keysPressed;
		std::vector<int> _keysReleased;
		glm::ivec2 _cursorPosition;
		std::weak_ptr<MimicCore> _mimicCore;
		bool _applicationRunning;
	};
}