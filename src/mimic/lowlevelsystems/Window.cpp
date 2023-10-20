#include "Window.h"
#include <iostream>

namespace Mimic
{
	Window::Window(std::string windowName) : WindowName(windowName)
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) throw std::runtime_error("Failed to initialize SDL");
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		SDL_assert(_window != NULL);

		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		SDL_assert(_renderer != NULL);

		_glContext = SDL_GL_CreateContext(_window);
	}

	Window::~Window() 
	{
		if (_glContext) SDL_GL_DeleteContext(_glContext);
		if (_renderer) SDL_DestroyRenderer(_renderer);
		if(_window) SDL_DestroyWindow(_window);
	}

	std::shared_ptr<Window> Window::Initialise(std::string windowName)
	{
		return std::make_shared<Window>(windowName);
	}
}