#include "gui.h"

namespace MimicGUI
{
	// #############################################################################
	// GuiElement Functions:
	// #############################################################################
	void GuiElement::SetPosition(const glm::vec2& position)
	{
		_position = position;
	}

	void GuiElement::SetSize(const glm::vec2& size)
	{
		_size = size;
	}

	glm::vec2 GuiElement::GetPosition() const
	{
		return _position;
	}

	glm::vec2 GuiElement::GetSize() const
	{
		return _size;
	}

	// #############################################################################
	// Button Functions:
	// #############################################################################
	int _globalGUIId;

	std::shared_ptr<Button> Button::Initialise()
	{
		auto button = std::make_shared<Button>();
		button->_name = "Button";
		button->_position = glm::vec2(0.0f);
		button->_size = glm::vec2(1.0f);
		button->_hsvColour = glm::vec3(1.0f);
		button->_hsvHoverColour = glm::vec3(1.0f);
		button->_hsvActiveColour = glm::vec3(1.0f);
		button->_id = _globalGUIId;
		_globalGUIId++;
		return button;
	}

	std::shared_ptr<Button> Button::Initialise(const std::string& name)
	{
		auto button = std::make_shared<Button>();
		button->_name = name;
		button->_position = glm::vec2(0.0f);
		button->_size = glm::vec2(1.0f);
		button->_hsvColour = glm::vec3(1.0f);
		button->_hsvHoverColour = glm::vec3(1.0f);
		button->_hsvActiveColour = glm::vec3(1.0f);
		button->_id = _globalGUIId;
		_globalGUIId++;
		return button;
	}

	std::shared_ptr<Button> Button::Initialise(const std::string& name, const glm::vec2& pos, const glm::vec2& size)
	{
		auto button = std::make_shared<Button>();
		button->_name = name;
		button->_position = pos;
		button->_size = size;
		button->_hsvColour = glm::vec3(1.0f);
		button->_hsvHoverColour = glm::vec3(1.0f);
		button->_hsvActiveColour = glm::vec3(1.0f);
		button->_id = _globalGUIId;
		_globalGUIId++;
		return button;
	}

	void Button::SetColourHSV(const glm::vec3& colour)
	{
		_hsvColour = colour;
	}

	void Button::SetHoverColourHSV(const glm::vec3& colour)
	{
		_hsvHoverColour = colour;
	}

	void Button::SetActiveColourHSV(const glm::vec3& colour)
	{
		_hsvActiveColour = colour;
	}

	void Button::GuiUpdate()
	{
		ImGui::SetNextWindowPos(ImVec2(_position.x, _position.y));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin(std::to_string(_id).c_str(), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	
		ImGui::PushID(_id);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(_hsvColour.x, _hsvColour.y, _hsvColour.z));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(_hsvHoverColour.x, _hsvHoverColour.y, _hsvHoverColour.z));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(_hsvActiveColour.x, _hsvActiveColour.y, _hsvActiveColour.z));

		if (ImGui::Button(_name.c_str(), ImVec2(_size.x, _size.y))) if (OnPressed) OnPressed();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		ImGui::End();
	}

	void Button::SetName(const std::string& name)
	{
		_name = name;
	}

	std::string Button::GetName() const
	{
		return _name;
	}

	// #############################################################################
	// Text Functions:
	// #############################################################################
	std::shared_ptr<Text> Text::Initialise()
	{
		auto text = std::make_shared<Text>();
		text->_message = " ";
		text->_position = glm::vec2(0.0f);
		text->_size = glm::vec2(1.0f);
		text->_hsvTextColour = glm::vec3(0.0f, 1.0f, 1.0f);
		text->_id = _globalGUIId;
		_globalGUIId++;
		return text;
	}

	std::shared_ptr<Text> Text::Initialise(const std::string& message)
	{
		auto text = std::make_shared<Text>();
		text->_message = message;
		text->_position = glm::vec2(0.0f);
		text->_size = glm::vec2(1.0f);
		text->_hsvTextColour = glm::vec3(0.0f, 1.0f, 1.0f);
		text->_id = _globalGUIId;
		_globalGUIId++;
		return text;
	}

	std::shared_ptr<Text> Text::Initialise(const std::string& message, const glm::vec2& pos, const glm::vec2& size)
	{
		auto text = std::make_shared<Text>();
		text->_message = message;
		text->_position = pos;
		text->_size = size;
		text->_hsvTextColour = glm::vec3(0.0f, 1.0f, 1.0f);
		text->_id = _globalGUIId;
		_globalGUIId++;
		return text;
	}

	void Text::GuiUpdate()
	{
		// ImGui::SetNextWindowSizeConstraints(ImVec2(100.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::SetNextWindowPos(ImVec2(_position.x, _position.y));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin(std::to_string(_id).c_str(), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::SetWindowFontScale(_fontScale);
		ImGui::TextColored(ImVec4(_hsvTextColour.x, _hsvTextColour.y, _hsvTextColour.z, 1.0f), _message.c_str());
		ImGui::End();
	}

	std::string Text::GetMessage() const
	{
		return _message;
	}

	void Text::SetMessage(const std::string& message)
	{
		_message = message;
	}

	void Text::SetColourHSV(const glm::vec3& colour)
	{
		_hsvTextColour = colour;
	}

	void Text::SetFontScale(const float& fontScale)
	{
		_fontScale = fontScale;
	}
}