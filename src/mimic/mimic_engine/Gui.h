#pragma once
#include "Component.h"
#include <mimic_gui/Gui.h>

namespace MimicEngine
{
	// #############################################################################
	// Button Struct (Engine):
	// #############################################################################
	struct Button : Component
	{
		void Initialise() override;
		void Start() override;
		void Update() override;
		void GuiUpdate() override;

		void SetSize(const glm::vec2& size);
		void SetName(const std::string& name);

		glm::vec2 GetSize() const;
		void SetColourHSV(const glm::vec3& colour);
		void SetHoverColourHSV(const glm::vec3& colour);
		void SetActiveColourHSV(const glm::vec3& colour);

		std::string GetName() const;

		/// <summary> Called when button is pressed. </summary>
		std::function<void()> OnPressed;

	private:
		std::shared_ptr<MimicGUI::Button> _guiButton;
	};

	// #############################################################################
	// Text Struct (Engine):
	// #############################################################################
	struct Text : Component
	{
		void Initialise() override;
		void Start() override;
		void Update() override;
		void GuiUpdate() override;

		void SetSize(const glm::vec2& size);
		void SetMessage(const std::string& message);
		void SetFontScale(const float& fontScale);

		glm::vec2 GetSize() const;
		void SetColourHSV(const glm::vec3& colour);

		std::string GetMessage() const;

	private:
		std::shared_ptr<MimicGUI::Text> _guiText;
	};
}