#pragma once
#include <SDL/SDL.h>
#include <memory>
#include <string>

// #############################################################################
// window struct:
// #############################################################################

namespace Mimic
{
	struct Window
	{
		Window(std::string);
		~Window();

		std::string WindowName;
	private:
		friend struct MimicCore;

		std::shared_ptr<Window> Initialise(std::string windowName);
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _glContext;
	};
}