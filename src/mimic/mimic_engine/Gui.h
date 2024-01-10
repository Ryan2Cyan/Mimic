#pragma once
#include "Component.h"
#include <mimic_gui/Gui.h>

namespace MimicEngine
{
	/// <summary>
	/// Wrapper for MimicGUI::Button. The user can set the 'OnPressed' function to execute code whenever the
	/// button is interacted with. The Button's position adheres to the GameObject it's attached to.
	/// </summary>
	struct Button : Component
	{
		void SetSize(const glm::vec2& size);
		void SetName(const std::string& name);

		glm::vec2 GetSize() const;
		void SetColourHSV(const glm::vec3& colour);
		void SetHoverColourHSV(const glm::vec3& colour);
		void SetActiveColourHSV(const glm::vec3& colour);
		void SetFontScale(const float& fontScale);

		std::string GetName() const;

		/// <summary> Called when button is pressed. </summary>
		std::function<void()> OnPressed;

	private:
		friend struct GameObject;

		void Initialise() override;
		void Start() override;
		void Update() override;
		void GuiUpdate() override;

		std::shared_ptr<MimicGUI::Button> _guiButton;
	};

	/// <summary>
	/// Wrapper for MimicGUI::Text. Displays message within text box. Text box scales to the size of the text inputted. 
	/// The Text's position adheres to the GameObject it's attached to.
	/// </summary>
	struct Text : Component
	{
		void SetSize(const glm::vec2& size);
		void SetMessage(const std::string& message);
		void SetFontScale(const float& fontScale);

		glm::vec2 GetSize() const;
		void SetColourHSV(const glm::vec3& colour);

		std::string GetMessage() const;

	private:
		friend struct GameObject;

		void Initialise() override;
		void Start() override;
		void Update() override;
		void GuiUpdate() override;

		std::shared_ptr<MimicGUI::Text> _guiText;
	};
}