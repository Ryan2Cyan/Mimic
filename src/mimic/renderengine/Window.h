#pragma once
#include <SDL/SDL.h>
#include <memory>
#include <string>
#include <GLM/glm.hpp>

// #############################################################################
// window struct:
// #############################################################################

namespace MimicRender
{
	struct Window
	{
		~Window();
		static std::shared_ptr<Window> Initialise(const std::string& windowName);
		const glm::vec2 GetAspectRatio() const noexcept;
		void SwapWindow() const;

	private:

		glm::ivec2 _aspectRatio;
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _glContext;
		bool _initialised;
	};
}