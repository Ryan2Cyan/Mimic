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

		void Draw(std::shared_ptr<Shader> shader);

	private:
		std::vector<std::shared_ptr<Mesh>> _meshes;
		std::string _directory;

		/*void LoadModel(const std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);*/
	};
}