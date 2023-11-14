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
	const mat4 rotView = mat4(mat3(u_View));
	const vec4 clipPosition = u_Projection * rotView * vec4(localPosition, 1.0);

	gl_Position = clipPosition.xyww;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#type fragment
#version 430 core

in vec3 localPosition;

uniform samplerCube u_EnvironmentMap;

out vec4 fragColour;

void main()
{
	vec3 environmentColour = texture(u_EnvironmentMap, localPosition).rgb;
	environmentColour = pow(environmentColour, vec3(1.0 / 2.2));

	fragColour = vec4(environmentColour, 1.0);
}