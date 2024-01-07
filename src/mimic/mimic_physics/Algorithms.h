#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#define EPSILON 0.000001

namespace MimicPhysics
{
	/// <summary> Tomas_Akenine-Moller: Conducts a ray intersection check on three input vertices (triangle), returns true if
	/// the ray intersects the triangle, false otherwise. </summary>
	static bool IntersectTriangle(const glm::vec3& ori, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& intersection = glm::vec3(0.0f))
	{
		// Source: https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/raytri/raytri.c

		// Find vectors for two edges sharing vertex 0.
		const auto edge1 = v1 - v0;
		const auto edge2 = v2 - v0;

		// Begin calculating the determinan (used to calculate the 'u' parameter).
		const auto pVec = glm::cross(dir, edge2);

		// If the determinant is near zero, the ray lies in the plane of the triangle.
		const auto det = glm::dot(edge1, pVec);
		if (det > -EPSILON && det < EPSILON) return false;
		const auto invDet = 1.0 / det;

		// Calculate the distance from v0 to the ray's origin.
		const auto tVec = ori - v0;

		// Calculate the U parameter and test bounds.
		intersection.y = glm::dot(tVec, pVec) * invDet;
		if (intersection.y < 0.0 || intersection.y > 1.0) return false;

		// Test the V parameter.
		const auto qVec = glm::cross(tVec, edge1);

		// Calculate the V parameter and test the bounds.
		intersection.z = glm::dot(dir, qVec) * invDet;
		const auto yz = intersection.y + intersection.z;
		if (intersection.z < 0.0 || yz > 1.0) return false;

		// Calculate t, ray intersects triangle.
		intersection.x = glm::dot(edge2, qVec) * invDet;
		return true;
	}
}