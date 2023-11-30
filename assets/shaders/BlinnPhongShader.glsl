#type vertex
#version 430 core
// Source: https://learnopengl.com/Lighting/Basic-Lighting
// Source: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_LightSpaceMatrix;
// uniform mat4 u_NormalMatrix;

out vec3 fragPosition;
out vec3 normal;
out vec4 fragPositionLightSpace;

void main()
{
	const vec4 fragPositionVec4 = vec4(aPos, 1.0);
	fragPosition = vec3(u_Model * fragPositionVec4);
	fragPositionLightSpace = u_LightSpaceMatrix * u_Model * fragPositionVec4;
	normal = mat3(transpose(inverse(u_Model))) * normalize(aNormal);
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#type fragment
#version 430 core

in vec3 fragPosition;
in vec4 fragPositionLightSpace;
in vec3 normal;

uniform vec3 u_ObjectColour;
uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;
uniform float u_AmbientStrength;
// uniform float u_DiffuseStrength;
uniform float u_SpecularStrength;
uniform float u_Shininess;

uniform sampler2D u_ShadowMap;

out vec4 fragColour;

const float ShadowCalculation(const vec4 lightSpacePos, const vec3 lightDir)
{
	vec3 projectedCoords = (lightSpacePos.xyz / lightSpacePos.w) * 0.5 + 0.5;
	if(projectedCoords.z > 1.0) return 0.0;

	const float closestDepth = texture(u_ShadowMap, projectedCoords.xy).r;
	const float currentDepth = projectedCoords.z;
	const float shadowBias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	// percentage-closer filtering to create the illusion of higher resolution shadows:
	float shadow = 0.0;
	const vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			const float pcfDepth = texture(u_ShadowMap, projectedCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - shadowBias > pcfDepth ? 1.0 : 0.0;
		}
	}
	return shadow /= 9.0;
}

void main()
{
	// calculate ambient light:
	const vec3 ambient = u_AmbientStrength * u_LightColour;

	// calculate diffuse light:
	const vec3 lightDirection = normalize(u_LightPosition - fragPosition);
	const vec3 diffuse = max(dot(normal, lightDirection), 0.0) * u_LightColour;

	// Blinn-Phong specular:
	const vec3 viewDirection = normalize(u_CameraPosition - fragPosition);
	const vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	const vec3 specular = u_SpecularStrength * pow(max(dot(normal, halfwayDirection), 0.0), u_Shininess) * u_LightColour;

	// Phong specular:
	// const vec3 reflectDirection = reflect(-lightDirection, normal);
	// const vec3 specular = u_SpecularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), u_Shininess) * u_LightColour;

	// calculate shadows:
	float shadow = ShadowCalculation(fragPositionLightSpace, lightDirection);

	// generate fragment colour:
	vec3 resultColour = (ambient + (1.0 - shadow) * (diffuse + specular)) * u_ObjectColour;

    fragColour = vec4(resultColour, 1.0);
}