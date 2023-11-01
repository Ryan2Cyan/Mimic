#include "Window.h"
#include <utility/Logger.h>

namespace Mimic
{
	Window::Window(std::string windowName, const glm::vec2 aspectRatio) : WindowName(windowName), AspectRatio(aspectRatio)
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL: %", SDL_GetError());
			throw;
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, aspectRatio.x, aspectRatio.y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (_window == nullptr)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL_Window: %", SDL_GetError());
			throw;
		}

		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		if (_renderer == nullptr)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL_Renderer: %", SDL_GetError());
			throw;
		}

		_glContext = SDL_GL_CreateContext(_window);
		if (_glContext == nullptr)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL_GL_Context: %", SDL_GetError());
			throw;
		}
	}

	Window::~Window() 
	{
		if (_glContext) SDL_GL_DeleteContext(_glContext);
		if (_renderer) SDL_DestroyRenderer(_renderer);
		if(_window) SDL_DestroyWindow(_window);
	}

	std::shared_ptr<Window> Window::Initialise(const std::string& windowName, const glm::vec2 aspectRatio)
	{
		return std::make_shared<Window>(windowName, aspectRatio);
	}
}