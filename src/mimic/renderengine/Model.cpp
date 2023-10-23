#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb_image.h>
#include <GL/glew.h>

// Source: https://learnopengl.com/Model-Loading/Model

namespace Mimic
{
	Model::Model(const char* modelPath) : _directory(modelPath) 
	{
		LoadModel(modelPath);
	}

	void Model::Draw(std::shared_ptr<Shader> shader)
	{
		const unsigned int length = _meshes.size();
		for (unsigned int i = 0; i < length; i++) _meshes[i].Draw(shader);
	}

	std::shared_ptr<Model> Model::Initialise(const char* modelPath)
	{
		return std::make_shared<Model>(modelPath);
	}

	unsigned int Model::LoadTextureFromFile(const char* path, const std::string& directory, bool gamma = false)
	{
		const std::string fileName = directory + '/' + std::string(path);

		unsigned int textureId;
		glGenTextures(1, &textureId);

		int width;
		int height;
		int componentsN;
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &componentsN, 0);
		if (data)
		{
			GLenum format;
			switch (componentsN)
			{
				case 1: 
				{ 
					format = GL_RED; 
					break;
				}
				case 3:
				{
					format = GL_RGB;
					break;
				}
				case 4:
				{
					format = GL_RGBA;
					break;
				}
				default: break;
			}

			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else std::cout << "Texture failed to load at path: " << path << std::endl;
		
		stbi_image_free(data);
		return textureId;
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
			
			vertices.push_back(newVertex);
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

			const std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			const std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}
	
	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		// convert aiTextureType into Mimic::Texture type:
		std::vector<Texture> textures;

		const unsigned int length = material->GetTextureCount(type);
		for (unsigned int i = 0; i < length; i++)
		{
			aiString tempStr;
			material->GetTexture(type, i, &tempStr);
			bool skip = false;
			for (unsigned int j = 0; j < _loadedTextures.size(); j++)
			{
				// check if we have already loaded the material texture:
				if (std::strcmp(_loadedTextures[j].Path.data(), tempStr.C_Str()) == 0)
				{
					textures.push_back(_loadedTextures[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				const char* tempCStr = tempStr.C_Str();
				Texture newTexture( LoadTextureFromFile(tempCStr, _directory), typeName, tempCStr);
				textures.push_back(newTexture);
				_loadedTextures.push_back(newTexture);
			}
		}
		return textures;
	}
}