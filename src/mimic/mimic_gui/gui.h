#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <glm/glm.hpp>
#include <functional>
#include <string>
#include <memory>

static int _globalGUIId;

namespace MimicGUI
{// #############################################################################
	// GuiElement Struct:
	// #############################################################################
	struct GuiElement
	{
		virtual void GuiUpdate() = 0;

		void SetPosition(const glm::vec2& position);
		void SetSize(const glm::vec2& size);

		glm::vec2 GetPosition() const;
		glm::vec2 GetSize() const;

	protected:
		glm::vec2 _position;
		glm::vec2 _size;
	};
	

	// #############################################################################
	// Image Struct:
	// #############################################################################
	struct Image : GuiElement
	{

	};

	// #############################################################################
	// Button Struct:
	// #############################################################################
	struct Button : GuiElement
	{
		static std::shared_ptr<Button> Initialise();
		static std::shared_ptr<Button> Initialise(const std::string& name);
		static std::shared_ptr<Button> Initialise(const std::string& name, const glm::vec2& pos, const glm::vec2& size);

		void GuiUpdate() override;

		/// <summary> Called when button is pressed. </summary>
		std::function<void()> OnPressed;

		void SetName(const std::string& name);
		void SetColourHSV(const glm::vec3& colour);
		void SetHoverColourHSV(const glm::vec3& colour);
		void SetActiveColourHSV(const glm::vec3& colour);

		std::string GetName() const;
	private:
		std::string _name;
		glm::vec3 _hsvColour;
		glm::vec3 _hsvHoverColour;
		glm::vec3 _hsvActiveColour;
		int _id = 0;
	};

	// #############################################################################
	// Text Struct:
	// #############################################################################
	struct Text : GuiElement
	{
		static std::shared_ptr<Text> Initialise();
		static std::shared_ptr<Text> Initialise(const std::string& message);
		static std::shared_ptr<Text> Initialise(const std::string& message, const glm::vec2& pos, const glm::vec2& size);

		void GuiUpdate() override;

		std::string GetMessage() const;

		void SetMessage(const std::string& message);
		void SetColourHSV(const glm::vec3& colour);
		void SetFontScale(const float& fontScale);

	private:
		std::string _message;
		glm::vec3 _hsvTextColour;
		float _fontScale = 1.0f;
		int _id = 0;
	};

	// #############################################################################
	// Slider Struct:
	// #############################################################################
	struct Slider : GuiElement
	{

	};
}