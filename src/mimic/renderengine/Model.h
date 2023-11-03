#pragma once

#include <lowlevelsystems/Resource.h>
#include <lowlevelsystems/Component.h>
#include <renderengine/Mesh.h>
#include <memory>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Mimic
{
	// #############################################################################
	// model stuct:
	// #############################################################################
	struct Shader;
	struct Texture;
	struct Mesh;

	struct Model : Resource
	{
	private:
		friend struct ResourceManager;
		friend struct ModelRenderer;

		const int Load(const std::string& path) override;
		void ProcessNode(aiNode* node, const aiScene* scene);
		const std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		const void LoadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const std::string&& typeName);
		
		std::vector<std::shared_ptr<Texture>> _materialTextures;
		std::vector<std::shared_ptr<Mesh>> _meshes;
	};
}