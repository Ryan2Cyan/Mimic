#pragma once

#include <lowlevelsystems/Mimic.h>
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

	struct Model
	{
		explicit Model(const char* modelPath);

		// KARSTEN ADVICE: Load model, store in list of paths:
		std::shared_ptr<Model> Initialise(const char* modelPath);
		void Draw(std::shared_ptr<Shader> shader);
		std::shared_ptr<Component> GetComponent() const;

		std::weak_ptr<Component> Component;

	private:
		friend struct ModelRenderer; 

		std::vector<Mesh> _meshes;
		std::vector<Texture> _loadedTextures;
		std::string _directory;
		std::weak_ptr<Model> _self;

		unsigned int LoadTextureFromFile(const char* path, const std::string& directory, bool gamma);
		void LoadModel(const std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}