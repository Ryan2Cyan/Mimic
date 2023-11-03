#type vertex
#version 430 core  

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColour;
out vec2 TexCoord;
out vec3 FragPos;

void main() 
{ 
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
}


#type fragment
#version 430 core 
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;
uniform sampler2D height;

void main()
{
    vec4 resultTexture;
    resultTexture += texture2D(diffuse, TexCoord);
    resultTexture += texture2D(specular, TexCoord);
    resultTexture += texture2D(normal, TexCoord);
    resultTexture += texture2D(height, TexCoord);
    
    FragColor = clamp(resultTexture, 0.0f, 1.0f);
}
