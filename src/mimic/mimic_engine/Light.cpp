#include "Light.h"
#include "MimicCore.h"

namespace MimicEngine
{
	std::shared_ptr<DirectLight> DirectLight::Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour)
	{
		std::shared_ptr<DirectLight> directLight = std::make_shared<DirectLight>();

		directLight->_renderDirectLight = MimicRender::DirectLight::Initialise(position, direction, colour);
		MimicCore::AddDirectLight(directLight);

		return directLight;
	}

	void DirectLight::SetPosition(const glm::vec3& position)
	{
		_renderDirectLight->Position = position;
	}

	void DirectLight::SetDirection(const glm::vec3& direction)
	{
		_renderDirectLight->Direction = direction;
	}

	void DirectLight::SetColour(const glm::vec3& colour)
	{
		_renderDirectLight->Colour = colour;
	}
}