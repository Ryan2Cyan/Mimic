#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>

namespace MimicRender
{
	struct Camera;
}

namespace MimicEngine
{
	/// <summary>
	/// Camera: Wrapper for MimicRender::Camera. Performs most visual-output-based calculations, primarily view and projection
	/// matrix calculations. 
	/// </summary>
	struct Camera 
	{
		void SetPosition(const glm::vec3& position);
		void SetOrientation(const glm::vec3& orientation);
		void SetName(const std::string& name);

		glm::vec3 GetPosition() const;
		glm::vec3 GetOrientation() const;
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;
		std::string GetName() const;


	private:
		friend struct MimicCore;
		friend struct InputHandler;

		static std::shared_ptr<Camera> Initialise(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane = glm::vec2(0.1f, 100.0f));

		std::shared_ptr<MimicRender::Camera> _renderCamera;
		std::string _name;
		void Update();
	};
}