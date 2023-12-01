#pragma once
#include <GLM/glm.hpp>
#include <memory>

namespace MimicRender
{
	// Source: https://learnopengl.com/Lighting/Light-casters
	// #############################################################################
	// light struct:
	// #############################################################################
	struct Light
	{
		glm::vec3 Colour;
		glm::vec3 Position;
	};

	// #############################################################################
	// direct light struct:
	// #############################################################################
	struct RenderTexture;
	struct DirectLight : Light
	{
		static const std::shared_ptr<DirectLight> Initialise() noexcept;
		static const std::shared_ptr<DirectLight> Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) noexcept;
		const unsigned int GetDepthMapTextureId() const noexcept;
		const glm::mat4 GetLightMatrix() const noexcept;

		glm::vec3 Direction;

	private:
		friend struct ShadowMapper;
		std::shared_ptr<RenderTexture> _depthMapRT;
		glm::mat4 _lightMatrix;
	};


	// #############################################################################
	// point light struct:
	// #############################################################################
	struct PointLight : Light
	{
		static const std::shared_ptr<PointLight> Initialise() noexcept;
		static const std::shared_ptr<PointLight> Initialise(const glm::vec3& position, const glm::vec3& colour) noexcept;

		const float GetConstant() const noexcept;
		const float GetLinear() const noexcept;
		const float GetQuadratic() const noexcept;

	private:
		const float _constant = 1.0f;
		const float _linear = 0.35f;
		const float _quadratic = 0.44f;
	};
}