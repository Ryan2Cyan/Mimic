#include "Window.h"
#include <utility/Logger.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

namespace Mimic
{
	Window::Window(const std::string& windowName) : _windowName(windowName)
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL: %", SDL_GetError());
			throw;
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		MIMIC_LOG_INFO("[Mimic::Window] SDL initialisation successful.");

		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		_aspectRatio = glm::ivec2(displayMode.w, displayMode.h);

		_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _aspectRatio.x, _aspectRatio.y, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (_window == nullptr)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL_Window: %", SDL_GetError());
			throw;
		}
		MIMIC_LOG_INFO("[Mimic::Window] SDL_Window initialisation successful.");

		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		if (_renderer == nullptr)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL_Renderer: %", SDL_GetError());
			throw;
		}
		MIMIC_LOG_INFO("[Mimic::Window] SDL_Renderer initialisation successful.");

		_glContext = SDL_GL_CreateContext(_window);
		if (_glContext == nullptr)
		{
			MIMIC_LOG_FATAL("Failed to initialize SDL_GL_Context: %", SDL_GetError());
			throw;
		}
		MIMIC_LOG_INFO("[Mimic::Window] SDL_GL_Context initialisation successful.");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		const char* glslVersion = "#version 130";
		ImGui_ImplSDL2_InitForOpenGL(_window, _glContext);
		ImGui_ImplOpenGL3_Init(glslVersion);
		MIMIC_LOG_INFO("[Mimic::Window] ImGui initialisation successful.");

		MIMIC_LOG_INFO("[Mimic::Window] Initialisation successful.");
	}

	Window::~Window() 
	{
		if (_glContext)
		{
			SDL_GL_DeleteContext(_glContext);
			MIMIC_LOG_INFO("[Mimic::Window] SDL_GL_Context destruction successful.");
		}
		else MIMIC_LOG_ERROR("[Mimic::Window] SDL_GL_Context null, destruction failed.");
		if (_renderer)
		{
			SDL_DestroyRenderer(_renderer);
			MIMIC_LOG_INFO("[Mimic::Window] SDL_Renderer destruction successful.");
		}
		else MIMIC_LOG_ERROR("[Mimic::Window] SDL_Renderer null, destruction failed.");
		if (_window)
		{
			SDL_DestroyWindow(_window);
			MIMIC_LOG_INFO("[Mimic::Window] SDL_Window destruction successful.");
		}
		else MIMIC_LOG_ERROR("[Mimic::Window] SDL_Window null, destruction failed.");

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	const glm::vec2 Window::GetAspectRatio() const noexcept
	{
		return _aspectRatio;
	}

	void Window::SwapWindow()
	{
		SDL_GL_SwapWindow(_window);
	}

	std::shared_ptr<Window> Window::Initialise(const std::string& windowName)
	{
		return std::make_shared<Window>(windowName);
	}
}