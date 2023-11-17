#type vertex
#version 430 core
// Source: https://learnopengl.com/PBR/IBL/Diffuse-irradiance

layout (location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 localPosition;

void main()
{
	localPosition = aPos;
	gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#type fragment
#version 430 core
#define PI 3.14159265359

in vec3 localPosition;

uniform samplerCube u_EnvironmentMap;

out vec4 fragColour;


void main()
{
	// sample direction equals the hemisphere's orientation:
	const vec3 normal = normalize(localPosition);
	vec3 irradiance = vec3(0.0);

	// discrete sampling of the hemisphere given the intergral's spherical coordinates:
	const vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), normal));
	const vec3 up = normalize(cross(normal, right));
	const float sampleDelta = 0.025;
	float numberOfSamples = 0.0;

	for(float phi = 0.0; phi < 0.2 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (tangent space):
			const vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));

			// tangent space to world space:
            const vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 

			irradiance += texture(u_EnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			numberOfSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / numberOfSamples);

	fragColour = vec4(irradiance, 1.0);
}