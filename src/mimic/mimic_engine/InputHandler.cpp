#include "InputHandler.h"
#include "MimicCore.h"

#include <SDL/SDL.h>

namespace MimicEngine
{
	std::shared_ptr<InputHandler> InputHandler::Initialise()
	{
		return std::make_shared<InputHandler>();
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
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					MimicCore::_applicationRunning = false;
					break;
				}

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
}