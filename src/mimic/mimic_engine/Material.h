#pragma once
#include <mimic_render/Texture.h>

#include <vector>

namespace MimicEngine
{
	/// <summary>
	/// Material: Base struct for all material classes. Materials are owned by the ModelRenderer component. 
	/// </summary>
	struct ResourceManager;
	struct Shader;
	struct Texture;
	struct GameObject;

	struct Material
	{
		virtual ~Material() = default;

		/// <summary> 
		/// This function<void> is passed into the renderer via a render object struct as a lambda.
		// This function is used primarily to set uniform values in the corresponding material shader. 
		/// </summary>
		virtual void OnDraw() = 0;

		/// <summary>
		/// Add texture material, binding it to a specific texture type. These materials will be rendered (via the
		/// ModelRenderer component) onto the model.
		/// </summary>
		/// <param name="texture">Texture to bind.</param>
		/// <param name="textureMapType">Texture binding point to be set (e.g. diffuse, specular, metallic, roughness...etc).</param>
		virtual void SetTextureMap(const std::shared_ptr<Texture>& texture, const MimicRender::TextureType& textureMapType) = 0;
		
	protected:
		friend struct ModelRenderer;
		
		std::weak_ptr<Shader> _shader;
		std::weak_ptr<GameObject> _gameObject;
	};

	struct PBRSubroutineHelper
	{
	private:
		friend struct PBRMaterial; 

		PBRSubroutineHelper() = default;
		PBRSubroutineHelper(const unsigned int& uniformId, const unsigned int& autoTextureId, const unsigned int& manualTextureId);

		unsigned int _uniformId;
		unsigned int _autoId;
		unsigned int _manualId;
	};

	/// <summary>
	/// PBRMaterial: Material that applies physically-based rendering to a model via the ModelRenderer component.
	/// </summary>
	struct PBRMaterial : Material
	{
		/// <summary>
		/// Add texture material, binding it to a specific texture type. These materials will be rendered (via the
		/// ModelRenderer component) onto the model.
		/// </summary>
		/// <param name="texture">Texture to bind.</param>
		/// <param name="textureMapType">Texture binding point to be set (e.g. diffuse, specular, metallic, roughness...etc).</param>
		void SetTextureMap(const std::shared_ptr<Texture>& texture, const MimicRender::TextureType& textureMapType) override;

		// PBR Literal parameter setters, these will be used if a corresponding texture map is
		// not provided:
		void SetAlbedo(const glm::vec3& albedo);
		void SetEmissive(const glm::vec3& emissive);
		void SetMetallic(const float& metallic);
		void SetRoughness(const float& roughness);
		void SetAmbientOcclusion(const float& ambientOcclusion);
		void SetAlpha(const float& alpha);

		// Set by the user, if true will not load textures, using literal pbr params instead:
		bool NoTextureMode;

	private:

		friend struct ModelRenderer;
		static std::shared_ptr<PBRMaterial> Initialise(const std::weak_ptr<GameObject>& gameObject);

		void OnDraw() override;

		std::vector<unsigned int> _subroutineIndices;

		PBRSubroutineHelper _albedoSubroutine;
		PBRSubroutineHelper _normalSubroutine;
		PBRSubroutineHelper _roughnessSubroutine;
		PBRSubroutineHelper _metallicSubroutine;

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
	};
}