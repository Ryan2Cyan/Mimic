#pragma once
#include <mimic_render/Camera.h>

#include <string>

namespace MimicEngine
{
	struct MimicRender::Camera;

	struct Camera 
	{
		void SetPosition(const glm::vec3& position);
		void SetOrientation(const glm::vec3& orientation);

		glm::vec3 GetPosition();
		glm::vec3 GetOrientation();

		std::string Name;

	private:
		friend struct MimicCore;
		static std::shared_ptr<Camera> Initialise(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane = glm::vec2(0.1f, 100.0f));

		std::shared_ptr<MimicRender::Camera> _renderCamera;
		void Update();
	};
}