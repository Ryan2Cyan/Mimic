#include "CubemapTexture.h"
#include <GL/glew.h>

namespace Mimic 
{
	// #############################################################################
	// cubemap texture:
	// #############################################################################
	const unsigned int CubeMapTexture::Initialise(const glm::vec2& aspectRatio, const bool generateMipMaps)
	{
		// generate cubemap color textures for each face (6) of the cubemap:
		unsigned int cubeMapTextureId;
		glGenTextures(1, &cubeMapTextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, aspectRatio.x, aspectRatio.y, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(generateMipMaps) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		return cubeMapTextureId;
	}
}