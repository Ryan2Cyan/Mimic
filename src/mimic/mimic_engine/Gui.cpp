#include "Gui.h"

namespace MimicEngine
{
	// #############################################################################
	// Button Functions (Engine):
	// #############################################################################
	void Button::Initialise()
	{
		_guiButton = MimicGUI::Button::Initialise("Button", GetGameObject()->Position, glm::vec2(100.0f, 80.0f));
	}

	void Button::Start()
	{

	}

	void Button::Update()
	{

	}

	void Button::GuiUpdate()
	{
		const auto gameObject = GetGameObject();
		_guiButton->SetPosition(gameObject->Position);
		_guiButton->OnPressed = OnPressed;
		_guiButton->GuiUpdate();
	}

	void Button::SetSize(const glm::vec2& size)
	{
		_guiButton->SetSize(size);
	}

	void Button::SetName(const std::string& name)
	{
		_guiButton->SetName(name);
	}

	void Button::SetColourHSV(const glm::vec3& colour)
	{
		_guiButton->SetColourHSV(colour);
	}

	void Button::SetHoverColourHSV(const glm::vec3& colour)
	{
		_guiButton->SetHoverColourHSV(colour);
	}

	void Button::SetActiveColourHSV(const glm::vec3& colour)
	{
		_guiButton->SetActiveColourHSV(colour);
	}

	glm::vec2 Button::GetSize() const
	{
		return _guiButton->GetSize();
	}

	std::string Button::GetName() const
	{
		return _guiButton->GetName();
	}

	// #############################################################################
	// Text Functions (Engine):
	// #############################################################################
	void Text::Initialise()
	{
		_guiText = MimicGUI::Text::Initialise(" ", GetGameObject()->Position, glm::vec2(100.0f, 80.0f));
	}

	void Text::Start()
	{

	}

	void Text::Update()
	{

	}

	void Text::GuiUpdate()
	{
		const auto gameObject = GetGameObject();
		_guiText->SetPosition(gameObject->Position);
		_guiText->GuiUpdate();
	}

	void Text::SetSize(const glm::vec2& size)
	{
		_guiText->SetSize(size);
	}

	void Text::SetMessage(const std::string& message)
	{
		_guiText->SetMessage(message);
	}

	void Text::SetColourHSV(const glm::vec3& colour)
	{
		_guiText->SetColourHSV(colour);
	}

	glm::vec2 Text::GetSize() const
	{
		return _guiText->GetSize();
	}

	void Text::SetFontScale(const float& fontScale)
	{
		_guiText->SetFontScale(fontScale);
	}

	std::string Text::GetMessage() const
	{
		return _guiText->GetMessage();
	}
}