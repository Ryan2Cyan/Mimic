#pragma once
#include <vector>
#include <list>
#include <memory>

namespace MimicEngine
{
	// #############################################################################
	// Input Handler stuct:
	// #############################################################################
	struct InputHandler
	{
		/// <summary> Return true if key is pressed or held, otherwise false. </summary>
		bool IsKey(const int key);

		/// <summary> Return true if key is pressed, otherwise false. </summary>
		bool IsKeyPressed(const int key);

		/// <summary> Return true if key was released this frame, otherwise false. </summary>
		bool IsKeyReleased(const int key);

	private:
		friend struct MimicCore;

		static std::shared_ptr<InputHandler> Initialise(bool& applicationRunning);

		void Update();
		void ClearTemp();

		std::list<int> _keys;
		std::vector<int> _keysPressed;
		std::vector<int> _keysReleased;
		bool _applicationRunning;
	};
}