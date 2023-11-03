#include "Model.h"
#include <utility/Logger.h>
#include <renderengine/Vertex.h>
#include <renderengine/Texture.h>
#include <lowlevelsystems/ResourceManager.h>
#include <iostream>

// Source: https://learnopengl.com/Model-Loading/Model

namespace Mimic
{
	typedef std::vector<std::shared_ptr<Texture>> textureVector;

	const int Model::Load(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene)
		{
			MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		if (!scene->mRootNode)
		{
			MIMIC_LOG_WARNING("[Model] Assimp importer encountered an error: %", importer.GetErrorString());
			return -1;
		}
		ProcessNode(scene->mRootNode, scene);
		return 0;
	}


	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all node's meshes:
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// do the same for each of its children:
		for (unsigned int i = 0; i < node->mNumChildren; i++) ProcessNode(node->mChildren[i], scene);
	}

	const std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// convert aiMesh into Mimic::Mesh: (vertex, normal, tex coords):
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const aiVector3D meshVertices = mesh->mVertices[i];
			const aiVector3D meshNormals = mesh->mNormals[i];

			Vertex newVertex(
				glm::vec3(meshVertices.x, meshVertices.y, meshVertices.z),
				glm::vec3(meshNormals.x, meshNormals.y, meshNormals.z),
				glm::vec2(0.0f)
			);

			if (mesh->mTextureCoords[0]) newVertex._textureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			vertices.push_back(newVertex);
		}

		// store face indices (each face representing a primitive/triangle):
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			const unsigned int faceIndicesLength = face.mNumIndices;
			for (unsigned int i = 0; i < faceIndicesLength; i++) indices.push_back(face.mIndices[i]);
		}

		std::shared_ptr<Texture> diffuseMap;
		std::shared_ptr<Texture> specularMap;
		std::shared_ptr<Texture> normalMap;
		std::shared_ptr<Texture> heightMap;

		// mesh contains material index - retrieve material from scene:
		if (mesh->mMaterialIndex >= 0)
		{
			const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			diffuseMap = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			specularMap = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			normalMap = LoadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
			heightMap = LoadMaterialTextures(material, aiTextureType_AMBIENT, "height");
		}

		std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(vertices, indices);
		newMesh->SetDiffuse(diffuseMap);
		newMesh->SetSpecular(specularMap);
		newMesh->SetNormal(normalMap);
		newMesh->SetHeight(heightMap);

		return newMesh;
	}

	const std::shared_ptr<Texture> Model::LoadMaterialTextures(const aiMaterial* material, const aiTextureType& type, const std::string&& typeName)
	{
		std::shared_ptr<Texture> loadedTexture;

		const unsigned int length = material->GetTextureCount(type);
		for (unsigned int i = 0; i < length; i++)
		{
			aiString aiPath;
			material->GetTexture(type, i, &aiPath);
			std::string texturePath = aiPath.C_Str();
			loadedTexture = GetResourceManager()->LoadResource<Texture>(texturePath);
			if (loadedTexture != nullptr)
			{
				loadedTexture->_type = typeName;
				return loadedTexture;
			}
			else MIMIC_LOG_WARNING("[Model] Unable to load texture of type: \"%\", from path: \"%\"", typeName, texturePath);
		}
		MIMIC_LOG_WARNING("[Model] Unable to load texture of type: \"%\"", typeName);
		return loadedTexture;
	}
}