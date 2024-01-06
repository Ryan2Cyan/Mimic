#include "InputHandler.h"
#include "MimicCore.h"
#include "Camera.h"
#include "mimic_render/Camera.h"
#include <mimic_utility/Math.h>

#include <SDL/SDL.h>

namespace MimicEngine
{
	std::shared_ptr<InputHandler> InputHandler::Initialise(bool& applicationRunning)
	{
		auto inputHandler = std::make_shared<InputHandler>();
		inputHandler->_applicationRunning = applicationRunning;
		return inputHandler;
	}

	bool InputHandler::IsKey(const int key)
	{
		if (_keys.size() < 1) return false;
		for (auto storedKey : _keys) if (storedKey == key) return true;
		return false;
	}

	bool InputHandler::IsKeyPressed(const int key)
	{
		if (_keysPressed.size() < 1) return false;

		for (auto storedKey : _keysPressed) if (storedKey == key) return true;
		return false;
	}

	bool InputHandler::IsKeyReleased(const int key)
	{
		if (_keysReleased.size() < 1) return false;

		for (auto storedKey : _keysReleased) if (storedKey == key) return true;
		return false;
	}

	void InputHandler::Update()
	{
		ClearTemp();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					_applicationRunning = false;
				}break;

				case SDL_MOUSEMOTION:
				{
					// Capture the mouse's current position on the screen.
					SDL_GetMouseState(&_cursorPosition.x, &_cursorPosition.y);
				}break;

				case SDL_KEYDOWN:
				{
					const auto key = event.key.keysym.sym;
					if (!IsKey(key))
					{
						_keys.push_back(key);
						_keysPressed.push_back(key);
						MIMIC_DEBUG_LOG("Pressed: %", key);
					}
				} break; 

				case SDL_KEYUP: 
				{ 
					const auto key = event.key.keysym.sym;
					_keys.remove(key);
					_keysReleased.push_back(key);
					MIMIC_DEBUG_LOG("Released: %", key);
					
				} break;
			}
		}
	}

	void InputHandler::ClearTemp()
	{
		_keysPressed.clear();
		_keysReleased.clear();
	}

	Ray InputHandler::MousePositionProject(const std::shared_ptr<Camera>& cam) const
	{
		if (!cam) return Ray();

		// Convert resolution into normalized device coordinates.
		const glm::vec2 r = _mimicCore.lock()->GetWindow()->GetAspectRatio();
		auto ndc = glm::vec4(0.0f, 0.0f, MimicUtility::ConvertToRange((float)_cursorPosition.x, 0.0f, r.x, -1.0f, 1.0f),
			MimicUtility::ConvertToRange((float)_cursorPosition.y, 0.0f, r.y, -1.0f, 1.0f));

		// Pass through the projection matrix.
		ndc = cam->_renderCamera->GetProjectionMatrix() * ndc;

		// Pass through the view matrix twice, one for the near plane, one for the far. 
		// These planes are distinguished by negating the z-axis.
		const auto v = cam->_renderCamera->GetViewMatrix();
		const auto n = v * glm::vec4(ndc.x, ndc.y, 1.0f, ndc.w);
		const auto f = v * glm::vec4(ndc.x, ndc.y, -1.0f, ndc.w);

		// Ray's origin is the near plane, the direction is the difference between the far and near plane.
		return Ray(n, f - n);
	}
}