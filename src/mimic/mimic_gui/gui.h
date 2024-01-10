#pragma once
#include <glm/glm.hpp>
#include <functional>
#include <string>
#include <memory>

static int _globalGUIId;

namespace MimicGUI
{
	/// <summary>
	/// GuiElement: Base class for GUI elements. Contains the position and size of the GUI element.
	/// </summary>
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
	

	/// <summary>
	/// Button: Pressable button GUI element. The user can set the 'OnPressed' function to execute code whenever the
	/// button is interacted with.
	/// </summary>
	struct Button : GuiElement
	{
		static std::shared_ptr<Button> Initialise();
		static std::shared_ptr<Button> Initialise(const std::string& name);
		static std::shared_ptr<Button> Initialise(const std::string& name, const glm::vec2& pos, const glm::vec2& size);

		void GuiUpdate() override;

		/// <summary> 
		/// Called when button is pressed. 
		/// </summary>
		std::function<void()> OnPressed;

		void SetName(const std::string& name);
		void SetColourHSV(const glm::vec3& colour);
		void SetHoverColourHSV(const glm::vec3& colour);
		void SetActiveColourHSV(const glm::vec3& colour);
		void SetFontScale(const float& fontScale);

		std::string GetName() const;

	private:

		std::string _name;
		glm::vec3 _hsvColour;
		glm::vec3 _hsvHoverColour;
		glm::vec3 _hsvActiveColour;
		float _fontScale = 1.0f;
		int _id = 0;
	};

	/// <summary>
	/// Text: Displays message within text box. Text box scales to the size of the text inputted.
	/// </summary>
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
}