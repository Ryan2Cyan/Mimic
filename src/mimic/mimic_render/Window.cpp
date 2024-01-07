#include "Window.h"
#include <mimic_utility/Logger.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <GL/glew.h>

namespace MimicRender
{
	std::shared_ptr<Window> Window::Initialise(const std::string& windowName)
	{
		std::shared_ptr<Window> window = std::make_shared<Window>();

		// Initialise the SDL library:
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		{
			MIMIC_LOG_FATAL("[MimicRender::Window] Failed to initialize SDL: %", SDL_GetError());
			return nullptr;
		}
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		MIMIC_LOG_INFO("[MimicRender::Window] SDL initialisation successful.");

		// Query the user monitors current display resolution and cache it:
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		const auto resolution = glm::ivec2(displayMode.w, displayMode.h);
		window->_aspectRatio = resolution;

		// Initialise SDL window using user's monitor resolution as the aspect:
		window->_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution.x, resolution.y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (window->_window == nullptr)
		{
			MIMIC_LOG_FATAL("[MimicRender::Window] Failed to initialize SDL_Window: %", SDL_GetError());
			return nullptr;
		}
		MIMIC_LOG_INFO("[MimicRender::Window] SDL_Window initialisation successful.");

		// Initialise SDL renderer:
		window->_renderer = SDL_CreateRenderer(window->_window, -1, SDL_RENDERER_ACCELERATED);
		if (window->_renderer == nullptr)
		{
			MIMIC_LOG_FATAL("[MimicRender::Window] Failed to initialize SDL_Renderer: %", SDL_GetError());
			return nullptr;
		}
		MIMIC_LOG_INFO("[Mimic::Window] SDL_Renderer initialisation successful.");

		// Initialise SDL OpenGL context:
		window->_glContext = SDL_GL_CreateContext(window->_window);
		if (window->_glContext == nullptr)
		{
			MIMIC_LOG_FATAL("[MimicRender::Window] Failed to initialize SDL_GL_Context: %", SDL_GetError());
			return nullptr;
		}
		MIMIC_LOG_INFO("[MimicRender::Window] SDL_GL_Context initialisation successful.");


		// Initialise ImGui library:
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		const char* glslVersion = "#version 130";
		ImGui_ImplSDL2_InitForOpenGL(window->_window, window->_glContext);
		ImGui_ImplOpenGL3_Init(glslVersion);
		MIMIC_LOG_INFO("[MimicRender::Window] ImGui initialisation successful.");

		// Initialise GLEW wrangler library:
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			MIMIC_LOG_FATAL("[MimicRender::Window] GLEW failed to initialise with message: %", glewGetErrorString(err));
			return nullptr;
		}
		MIMIC_LOG_INFO("[MimicRender::Window] Initialisation successful.");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glCullFace(GL_BACK);

		window->_initialised = true;
		return window;
	}

	Window::~Window() 
	{
		// Destroy components of SDL in reverse order they were initialised:
		if (_glContext)
		{
			SDL_GL_DeleteContext(_glContext);
			MIMIC_LOG_INFO("[MimicRender::Window] SDL_GL_Context destruction successful.");
		}
		else MIMIC_LOG_ERROR("[MimicRender::Window] SDL_GL_Context null, destruction failed.");
		if (_renderer)
		{
			SDL_DestroyRenderer(_renderer);
			MIMIC_LOG_INFO("[MimicRender::Window] SDL_Renderer destruction successful.");
		}
		else MIMIC_LOG_ERROR("[MimicRender::Window] SDL_Renderer null, destruction failed.");
		if (_window)
		{
			SDL_DestroyWindow(_window);
			MIMIC_LOG_INFO("[MimicRender::Window] SDL_Window destruction successful.");
		}
		else MIMIC_LOG_ERROR("[MimicRender::Window] SDL_Window null, destruction failed.");

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void Window::ResetViewPort() const noexcept
	{
		if (!_initialised) return;
		glViewport(0, 0, _aspectRatio.x, _aspectRatio.y);
	}

	glm::ivec2 Window::GetAspectRatio() const noexcept
	{
		if (!_initialised) glm::ivec2(0);
		return _aspectRatio;
	}

	void Window::ClearBuffers(const std::uint8_t& buffers)
	{
		if(buffers & MIMIC_CLEAR_COLOUR) glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		if (buffers & MIMIC_COLOUR_AND_DEPTH_BUFFERS) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (buffers & MIMIC_COLOUR_BUFFER) glClear(GL_COLOR_BUFFER_BIT);
		if (buffers & MIMIC_DEPTH_BUFFER) glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Window::SwapWindow() const
	{
		if(!_initialised) return;
		SDL_GL_SwapWindow(_window);
	}
}