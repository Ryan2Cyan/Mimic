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

in vec3 localPosition;

uniform sampler2D u_EquirectangularMap;

out vec4 fragColour;

 const vec2 inverseAtan = vec2(0.1591, 0.3183);

 const vec2 SampleSphericalMap(const vec3 vertex)
 {
 	vec2 uv = vec2(atan(vertex.z, vertex.x), asin(vertex.y));
 	uv *= inverseAtan;
 	uv += 0.5;
	return uv;
}

void main()
{
	const vec2 uv = SampleSphericalMap(normalize(localPosition));
	const vec3 colour = texture(u_EquirectangularMap, vec2(1.0)).rgb;
	fragColour = vec4(colour, 1.0);
}