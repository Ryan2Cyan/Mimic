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
		explicit Model();

		// KARSTEN ADVICE: Load model, store in list of paths:
		void Draw(std::shared_ptr<Shader> shader);
		std::shared_ptr<Component> GetComponent() const;

		std::weak_ptr<Component> Component;

	protected:

	private:
		friend struct ResourceManager;

		std::string _directory;
		std::vector<std::shared_ptr<Mesh>> _meshes;
		std::weak_ptr<Model> _self;

		const int Load(const std::string& path) override;
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		const std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}