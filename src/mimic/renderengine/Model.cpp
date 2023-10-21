#include "Model.h"

// Source: https://learnopengl.com/Model-Loading/Model
namespace Mimic
{
	Model::Model(const char* modelPath) : _directory(modelPath) {}

	void Model::Draw(std::shared_ptr<Shader> shader)
	{
		const unsigned int length = _meshes.size();
		for (unsigned int i = 0; i < length; i++) _meshes[i]->Draw(shader);
	}

	//void Model::LoadModel(const std::string path)
	//{
	//
	//}
	//void Model::ProcessNode(aiNode* node, const aiScene* scene)
	//{
	//
	//}
	//
	//Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	//{
	//
	//}
	//
	//std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	//{
	//
	//}
}