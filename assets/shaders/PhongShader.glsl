#type vertex
#version 430 core
// Source: https://learnopengl.com/Lighting/Basic-Lighting

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;

out vec3 fragPosition;
out vec3 normal;

void main()
{
	const vec4 fragmentPositionVec4 = vec4(aPos, 1.0);
	fragPosition = vec3(u_Model * fragmentPositionVec4);
	// normal = u_NormalMatrix * normalize(aNormal);
	normal = mat3(transpose(inverse(u_Model))) * normalize(aNormal);
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#type fragment
#version 430 core

in vec3 fragPosition;
in vec3 normal;

uniform vec3 u_ObjectColour;
uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;

out vec4 fragColour;

void main()
{
	// apply ambient light:
	const float ambientStrength = 0.8;
	const vec3 ambient = ambientStrength * u_LightColour;

	// apply diffuse light:
	const vec3 lightDirection = normalize(u_LightPosition - fragPosition);
	// const float diff = max(dot(normal, lightDirection), 0.0);
	const vec3 diffuse = max(dot(normal, lightDirection), 0.0) * u_LightColour;

	// generate fragment colour:
	const vec3 resultColour = (ambient * diffuse) * u_ObjectColour;

    fragColour = vec4(resultColour, 1.0);
}