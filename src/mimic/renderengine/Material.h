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

		void SetDiffuse(const int& diffuse);
		void SetSpecular(const int& specular);
		void SetNormal(const int& normal);
		void SetHeight(const int& height);
		std::shared_ptr<Shader> _shader;
		int _diffuseTexture = -1;
		int _specularTexture = -1;
		int _normalTexture = -1;
		int _heightTexture = -1;
	};

	// #############################################################################
	// basic material stuct:
	// #############################################################################
	struct BasicMaterial : Material
	{
	private:
		void OnDraw() override;
	
	};
}