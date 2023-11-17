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
		Window(const std::string& windowName, glm::ivec2 aspectRatio);
		~Window();
		const glm::vec2 GetAspectRatio() const noexcept;
		void SwapWindow();
	private:
		friend struct MimicCore;

		static std::shared_ptr<Window> Initialise(const std::string& windowName, const glm::ivec2& aspectRatio);

		glm::ivec2 _aspectRatio;
		std::string _windowName;
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _glContext;
	};
}