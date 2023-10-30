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
    vertexColour = vec4(1.0, 0.0, 0.0, 1.0);
}



#type fragment
#version 430 core 
out vec4 FragColor;

in vec4 vertexColour;
in vec2 TexCoord;

// should probably use count or enabled, not both?
struct material
{
    sampler2D texture;
    bool enabled;
};

uniform material materials[20]; 
uniform int materialsCount;

void main()
{
    // for each texture in materials, check if enabled, if it is mix it into the result frag colour.
    vec4 resultTexture;
    for (int i = 0; i < materialsCount; i++)
    {
        resultTexture = resultTexture + texture2D(materials[i].texture, TexCoord);
        if(materials[i].enabled)
        {
            resultTexture = resultTexture + texture2D(materials[i].texture, TexCoord);
        }
    }
    
    FragColor = clamp(resultTexture, 0.0f, 1.0f);
}
