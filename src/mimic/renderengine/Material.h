#pragma once
#include <string>
#include <memory>
#include <vector>
#include <GLM/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// material stuct:
	// #############################################################################
	struct ResourceManager;
	struct Shader;
	struct Texture;
	struct GameObject;

	struct Material
	{
		virtual ~Material() = default;
		virtual void OnDraw() = 0;

		
	protected:
		friend struct ModelRenderer;

		void SetShader(const std::shared_ptr<Shader>& shader);
		void SetDiffuse(const std::shared_ptr<Texture>& diffuse);
		void SetSpecular(const std::shared_ptr<Texture>& specular);
		void SetNormal(const std::shared_ptr<Texture>& normal);
		void SetHeight(const std::shared_ptr<Texture>& height);

		std::weak_ptr<Texture> _diffuseTexture;
		std::weak_ptr<Texture> _specularTexture;
		std::weak_ptr<Texture> _normalTexture;
		std::weak_ptr<Texture> _heightTexture;
		std::weak_ptr<Shader> _shader;
		std::weak_ptr<GameObject> _gameObject;
	};

	// #############################################################################
	// basic material stuct:
	// #############################################################################
	struct BasicMaterial : Material
	{	
		BasicMaterial();

	private:
		void OnDraw() override;
	};

	// #############################################################################
	// pbr stuct:
	// #############################################################################
	struct PBRMaterial : Material
	{
		PBRMaterial();

		void SetAlbedo(const glm::vec3& albedo);
		void SetEmissive(const glm::vec3& emissive);
		void SetMetallic(const float& metallic);
		void SetRoughness(const float& roughness);
		void SetAmbientOcclusion(const float& ambientOcclusion);
		void SetAlpha(const float& alpha);

		glm::vec3 Albedo;
		glm::vec3 Emissive;
		float Metallic; // unused
		float Roughness;
		float AmbientOcclusion;
		float Alpha;
		bool ManualMode; // if true will not load textures and passes in member pbr params
	private:
		std::vector<unsigned int> _subroutineIndices;

		unsigned int _albedoSubroutineUniform;
		unsigned int _autoAlbedo;
		unsigned int _manualAlbedo;

		unsigned int _normalSubroutineUniform;
		unsigned int _autoNormal;
		unsigned int _manualNormal;

		unsigned int _roughnessSubroutineUniform;
		unsigned int _autoRoughness;
		unsigned int _manualRoughness;
		void OnDraw() override;
	};
}