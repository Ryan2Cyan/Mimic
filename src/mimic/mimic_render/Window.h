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

		/// <summary>
		/// Reset the viewport to this window's cached aspect radio.
		/// </summary>
		void ResetViewPort() const noexcept;

		/// <summary>
		/// Return the aspect radio cached by this window.
		/// </summary>
		glm::ivec2 GetAspectRatio() const noexcept;

		/// <summary>
		/// 
		/// </summary>
		void ClearBuffers(const std::uint8_t& buffers = MIMIC_CLEAR_COLOUR | MIMIC_COLOUR_AND_DEPTH_BUFFERS);

		/// <summary>
		/// Used in the default OpenGL double buffer context. Swaps render buffers to display the twin
		/// buffer with the full frame's display data written to it.
		/// </summary>
		void SwapWindow() const;

		static const std::uint8_t MIMIC_CLEAR_COLOUR = 0b1;
		static const std::uint8_t MIMIC_COLOUR_AND_DEPTH_BUFFERS = 0b1;
		static const std::uint8_t MIMIC_COLOUR_BUFFER = 0b10;
		static const std::uint8_t MIMIC_DEPTH_BUFFER = 0b100;

		static const std::uint32_t MIMIC_WRAPT_REPEAT = 0b1'000;
	private:

		glm::ivec2 _aspectRatio;
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_GLContext _glContext;
		bool _initialised;
	};
}