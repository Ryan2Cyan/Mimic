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

		std::shared_ptr<Model> Initialise(const char* modelPath);
		void Draw(Shader& shader);

	private:
		std::vector<Mesh> _meshes;
		std::vector<Texture> _loadedTextures;
		std::string _directory;

		unsigned int LoadTextureFromFile(const char* path, const std::string& directory, bool gamma);
		void LoadModel(const std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}