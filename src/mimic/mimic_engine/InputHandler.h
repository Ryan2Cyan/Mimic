#pragma once
#include "MimicCore.h"

#include <vector>
#include <list>
#include <memory>
#include <GLM/glm.hpp>

namespace MimicEngine
{
	// #############################################################################
	// Ray Str uct:
	// #############################################################################
	struct Ray
	{
		Ray() : Origin(glm::vec3(0.0f)), Direction(glm::vec3(0.0f)) {}
		Ray(const glm::vec3& ori, const glm::vec3& dir) : Origin(ori), Direction(dir) {}
		glm::vec3 Origin;
		glm::vec3 Direction;
	};

	// #############################################################################
	// Input Handler Struct:
	// #############################################################################
	struct InputHandler
	{
		/// <summary> Return true if key is pressed or held, otherwise false. </summary>
		bool IsKey(const int key);

		/// <summary> Return true if key is pressed, otherwise false. </summary>
		bool IsKeyPressed(const int key);

		/// <summary> Return true if key was released this frame, otherwise false. </summary>
		bool IsKeyReleased(const int key);

		glm::ivec2 GetCursorPosition() const;
		Ray MousePositionProject(const std::shared_ptr<Camera>& cam, const glm::ivec2& cursorPos) const;

	private:
		friend struct MimicCore;

		static std::shared_ptr<InputHandler> Initialise(bool& applicationRunning);

		void Update();
		void ClearTemp();

		std::list<int> _keys;
		std::vector<int> _keysPressed;
		std::vector<int> _keysReleased;
		glm::ivec2 _cursorPosition;
		std::weak_ptr<MimicCore> _mimicCore;
		bool _applicationRunning;
	};
}