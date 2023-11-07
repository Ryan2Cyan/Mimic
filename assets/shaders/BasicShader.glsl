#type vertex
#version 430 core  

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;

void main() 
{ 
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#type fragment
#version 430 core 
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
uniform sampler2D u_Normal;
uniform sampler2D u_Height;

void main()
{
    vec4 resultTexture;
    resultTexture += texture2D(u_Diffuse, TexCoord);
    resultTexture += texture2D(u_Specular, TexCoord);
    resultTexture += texture2D(u_Normal, TexCoord);
    resultTexture += texture2D(u_Height, TexCoord);
    
    FragColor = clamp(resultTexture, 0.0f, 1.0f);
}
