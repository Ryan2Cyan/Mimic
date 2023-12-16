#pragma once
#include <mimic_render/Texture.h>

#include <string>
#include <memory>
#include <vector>
#include <GLM/glm.hpp>

namespace MimicEngine
{
	// #############################################################################
	// Material stuct:
	// #############################################################################
	struct ResourceManager;
	struct Shader;
	struct Texture;
	struct GameObject;

	struct Material
	{
		virtual ~Material() = default;
		virtual void OnDraw() = 0;

		/// <summary>
		/// Add texture material, binding it to a specific texture type. These materials will be rendered (via the
		/// ModelRenderer component) onto the model.
		/// </summary>
		virtual void SetTextureMap(const std::shared_ptr<Texture>& texture, MimicRender::TextureType type) = 0;
		
	protected:
		friend struct ModelRenderer;

		std::weak_ptr<Shader> _shader;
		std::weak_ptr<GameObject> _gameObject;
	};

	//// #############################################################################
	//// basic material stuct:
	//// #############################################################################
	//struct BasicMaterial : Material
	//{	
	//	BasicMaterial();
	//	void SetDiffuse(const std::shared_ptr<Texture>& diffuse);
	//	void SetSpecular(const std::shared_ptr<Texture>& specular);
	//	void SetNormal(const std::shared_ptr<Texture>& normal);
	//	void SetHeight(const std::shared_ptr<Texture>& height);

	//protected:
	//	friend struct ModelRenderer;
	//	void SetTextureMap(const std::shared_ptr<Texture>& texture) override;

	//private:
	//	std::weak_ptr<Texture> _diffuseTexture;
	//	std::weak_ptr<Texture> _specularTexture;
	//	std::weak_ptr<Texture> _normalTexture;
	//	std::weak_ptr<Texture> _heightTexture;
	//	void OnDraw() override;
	//};

	// #############################################################################
	// pbr material stuct:
	// #############################################################################
	struct PBRMaterial : Material
	{
		// PBRMaterial();
		static std::shared_ptr<PBRMaterial> Initialise();
		void SetTextureMap(const std::shared_ptr<Texture>& texture, MimicRender::TextureType type) override;

		void SetAlbedo(const glm::vec3& albedo);
		void SetEmissive(const glm::vec3& emissive);
		void SetMetallic(const float& metallic);
		void SetRoughness(const float& roughness);
		void SetAmbientOcclusion(const float& ambientOcclusion);
		void SetAlpha(const float& alpha);

		bool ManualMode; // if true will not load textures and passes in member pbr params

	protected:
		friend struct ModelRenderer;

	private:
		std::vector<unsigned int> _subroutineIndices;
		
		glm::vec3 _albedo;
		glm::vec3 _emissive;

		std::weak_ptr<Texture> _albedoTexture;
		std::weak_ptr<Texture> _metallicTexture;
		std::weak_ptr<Texture> _roughnessTexture;
		std::weak_ptr<Texture> _normalTexture;

		float _metallic;
		float _roughness;
		float _ambientOcclusion;
		float _alpha;

		unsigned int _albedoSubroutineUniform;
		unsigned int _autoAlbedo;
		unsigned int _manualAlbedo;

		unsigned int _normalSubroutineUniform;
		unsigned int _autoNormal;
		unsigned int _manualNormal;

		unsigned int _roughnessSubroutineUniform;
		unsigned int _autoRoughness;
		unsigned int _manualRoughness;

		unsigned int _metallicSubroutineUniform;
		unsigned int _autoMetallic;
		unsigned int _manualMetallic;
		// void OnDraw() override;
	};


	//// #############################################################################
	//// cubemap stuct:
	//// #############################################################################
	//struct CubeMapMaterial : Material
	//{
	//	CubeMapMaterial();
	//	void SetSourceTexture(const std::shared_ptr<Texture>& texture);
	//private:
	//	void OnDraw() override;

	//	std::weak_ptr<Texture> _sourceTexture;
	//};
}