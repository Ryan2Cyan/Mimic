#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb_image.h>

// Source: https://learnopengl.com/Model-Loading/Model

namespace Mimic
{
	Model::Model(const char* modelPath) : _directory(modelPath) {}

	void Model::Draw(std::shared_ptr<Shader> shader)
	{
		const unsigned int length = _meshes.size();
		for (unsigned int i = 0; i < length; i++) _meshes[i].Draw(shader);
	}

	void Model::LoadModel(const std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		if(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		if(!scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		
		_directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all node's meshes:
		unsigned int length = node->mNumMeshes;
		for (unsigned int i = 0; i < length; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// do the same for each of its children:
		length = node->mNumChildren;
		for (unsigned int i = 0; i < length; i++) ProcessNode(node->mChildren[i], scene);
	}
	
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// convert aiMesh into Mimic::Mesh: (vertex, normal, tex coords):
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		unsigned int length = mesh->mNumVertices;
		for (unsigned int i = 0; i < length; i++)
		{
			const aiVector3D meshVertices = mesh->mVertices[i];
			const aiVector3D meshNormals = mesh->mNormals[i];

			Vertex newVertex(
				glm::vec3(meshVertices.x, meshVertices.y, meshVertices.z),
				glm::vec3(meshNormals.x, meshNormals.y, meshNormals.z),
				glm::vec2(0.0f)
			);

			if (mesh->mTextureCoords[0]) newVertex.TextureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			
			Mesh newMesh(vertices, indices, textures);
			return newMesh;
		}

		// store face indices (each face representing a primitive/triangle):
		length = mesh->mNumFaces;
		for (unsigned int i = 0; i < length; i++)
		{
			aiFace face = mesh->mFaces[i];
			const unsigned int faceIndicesLength = face.mNumIndices;
			for (unsigned int i = 0; i < faceIndicesLength; i++) indices.push_back(face.mIndices[i]);
		}

		// mesh contains material index - retrieve material from scene:
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			const std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			
			const std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
	}
	
	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		// convert aiTextureType into Mimic::Texture type:
		std::vector<Texture> textures;

		//const unsigned int length = material->GetTextureCount(type);
		//for (unsigned int i = 0; i < length; i++)
		//{
		//	aiString tempStr;
		//	material->GetTexture(type, i, &tempStr);
		//	Texture texture;
		//	// NOTE: Implement texture loader:
		//	//texture.Id = TextureFromFile
		//	
		//}
		return textures;
	}
}