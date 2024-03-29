#type vertex
#version 430 core
// Source: https://learnopengl.com/Advanced-OpenGL/Cubemaps
// Source: https://www.youtube.com/watch?v=8sVvxeKI9Pk&t=152s

layout(location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 texCoords;

void main()
{
	const vec4 pos = u_Projection * u_View * vec4(aPos, 1.0f);
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
	gl_Position = pos.xyww;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#type fragment
#version 430 core

in vec3 texCoords;

uniform samplerCube u_Skybox;

out vec4 fragColour;

void main()
{
    fragColour = texture(u_Skybox, texCoords);
}