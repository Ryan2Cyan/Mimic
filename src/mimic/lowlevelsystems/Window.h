#pragma once
#include <SDL/SDL.h>
#include <memory>
#include <string>
#include <GLM/glm.hpp>

// #############################################################################
// window struct:
// #############################################################################

namespace Mimic
{
	struct Window
	{
		Window(std::string, glm::vec2 aspectRatio);
		~Window();

		glm::vec2 AspectRatio;
		std::string WindowName;
	private:
		friend struct MimicCore;

		std::shared_ptr<Window> Initialise(const std::string& windowName, const glm::vec2 aspectRatio);
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _glContext;
	};
}