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
		const glm::vec2 GetAspectRatio() const noexcept;

	private:
		friend struct MimicCore;

		static std::shared_ptr<Window> Initialise(const std::string& windowName, const glm::vec2 aspectRatio);

		glm::vec2 _aspectRatio;
		std::string _windowName;
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _glContext;
	};
}