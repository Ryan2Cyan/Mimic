#pragma once
#include <GLM/glm.hpp>
#include <memory>

namespace MimicRender
{
	// Source: https://learnopengl.com/Lighting/Light-casters


	/// <summary>
	/// Light: Base class for all lights. Contains a colour (RGB) and position.
	/// </summary>
	struct Light
	{
		glm::vec3 Colour;
		glm::vec3 Position;
	};

	/// <summary>
	/// DirectLight: User can set the position, direction and colour (RGB). Note that shadow maps are 
	/// dependent on the position, whereas rendered light is not.
	/// </summary>
	struct RenderTexture;
	struct DirectLight : Light
	{
		static std::shared_ptr<DirectLight> Initialise() noexcept;
		static std::shared_ptr<DirectLight> Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) noexcept;

		/// <summary> 
		/// Get the ID of the depth map stores within the light. This value can only be assigned by a
		/// ShadowMapper::RenderDirectLightDepthMaps() call. 
		/// </summary>
		unsigned int GetDepthMapTextureId() const noexcept;

		/// <summary> 
		/// Get the 4x4 light matrix used to render the scene from the light's prespective for shadow
		/// mapping. Calculated by multiplying the lightProjection and lightView matricies together. 
		/// </summary>
		glm::mat4 GetLightMatrix() const noexcept;

		glm::vec3 Direction;

	private:
		friend struct ShadowMapper;

		std::shared_ptr<RenderTexture> _depthMapRT;
		glm::mat4 _lightMatrix;
	};


	/// <summary>
	/// PointLight: Position of this light is all that's important, fall off and additional calculations are handled
	/// in the shaders. The Constant, Linear, and Quadratic values are hard coded.
	/// </summary>
	struct PointLight : Light
	{
		static std::shared_ptr<PointLight> Initialise() noexcept;
		static std::shared_ptr<PointLight> Initialise(const glm::vec3& position, const glm::vec3& colour) noexcept;

		/// <summary> 
		/// Gets the constant term for the light attenuation formula. 
		/// </summary>
		float GetConstant() const noexcept;

		/// <summary>
		/// Gets the linear term for the light attenuation formula. 
		/// </summary>
		float GetLinear() const noexcept;

		/// <summary>
		/// Gets the quadratic term for the light attenuation formula. 
		/// </summary>
		float GetQuadratic() const noexcept;

	private:
		const float _constant = 1.0f;
		const float _linear = 0.35f;
		const float _quadratic = 0.44f;
	};
}