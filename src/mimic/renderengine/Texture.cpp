#include "Texture.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb_image.h>
#include <iostream>

namespace Mimic
{
	Texture::Texture() : Id(-1), Type(""), Path("") {}
	Texture::Texture(const unsigned int id, const std::string type, const std::string path) : Id(id), Type(type), Path(path) {}

	void Texture::Load(const std::string& path)
	{
	
	}

	unsigned int Texture::LoadTextureFromFile(const char* path, const std::string& directory, bool gamma)
	{
		const std::string fileName = directory + '/' + std::string(path);

		glGenTextures(1, &Id);

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

			glBindTexture(GL_TEXTURE_2D, Id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else std::cout << "Texture failed to load at path: " << path << std::endl;

		stbi_image_free(data);
		return Id;
	}
}