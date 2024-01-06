#pragma once
#include <GLM/glm.hpp>

namespace MimicRender
{
	// #############################################################################
	// Triangle Struct:
	// #############################################################################
	struct Triangle
	{
		Triangle();
		Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
	};
}