#pragma once
#include <string>
#include <memory>
#include <GLM/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// material stuct:
	// #############################################################################
	struct ResourceManager;
	struct Shader;
	struct Texture;

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
		std::weak_ptr<Texture> _specularTexture; // used as specular (map_ks) and roughness (map_Ns)
		std::weak_ptr<Texture> _normalTexture;
		std::weak_ptr<Texture> _heightTexture;
		std::weak_ptr<Shader> _shader;
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
	enum PbrMode
	{
		Manual, AutoTexture
	};

	struct PBRMaterial : Material
	{
		PBRMaterial();

		void SetAlbedo(const glm::vec3& albedo);
		void SetEmissive(const glm::vec3& emissive);
		void SetMetallic(const float& metallic);
		void SetRoughness(const float& roughness);
		void SetAmbientOcclusion(const float& ambientOcclusion);
		void SetAlpha(const float& alpha);
		void SetPBRMode(const PbrMode& pbrMode);

	private:
		glm::vec3 _albedo;
		glm::vec3 _emissive;
		float _metallic;
		float _roughness;
		float _ambientOcclusion;
		float _alpha;
		PbrMode _pbrMode;
		unsigned int _autoTextureSubRoutine;
		unsigned int _manualSubRoutine;
		void OnDraw() override;
	};
}