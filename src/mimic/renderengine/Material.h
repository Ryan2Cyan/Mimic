#pragma once
#include <string>
#include <memory>

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
		std::weak_ptr<Texture> _specularTexture;
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
}