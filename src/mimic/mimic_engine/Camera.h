#pragma once
#include <mimic_render/Camera.h>

#include <string>

namespace MimicEngine
{
	struct MimicRender::Camera;

	struct Camera 
	{
		static std::shared_ptr<Camera> Initialise(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane = glm::vec2(0.1f, 100.0f));

		void SetPosition(const glm::vec3& position);
		void SetOrientation(const glm::vec3& orientation);
		std::string Name;

	private:
		friend struct MimicCore;

		void Update();

		std::shared_ptr<MimicRender::Camera> _renderCamera;
	};
}