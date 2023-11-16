#pragma once
#include <glm/glm.hpp>

namespace Mimic
{
	// #############################################################################
	// cubemap texture:
	// #############################################################################
	struct CubeMapTexture 
	{
	private:
		friend struct EnvironmentCubeMap;

		static const unsigned int Initialise(const glm::vec2& aspectRatio, const bool generateMipMaps = false);
	};
}