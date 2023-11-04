#type vertex
#version 430 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec4 u_WorldSpaceLightPos;

out vec3 viewDirectionNormalV;
out vec3 lightPositionV;
out vec3 vertexPositionV;
out vec2 texCoord;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * aPos;

	vertexPositionV = vec3(u_View * u_Model * aPos);
	lightPositionV = vec3(u_View * u_WorldSpaceLightPos);
	viewDirectionNormalV = mat3(u_View * u_Model) * aNormal;
	texCoord = aTexCoord;
}




#type fragment
#version 430 core
// Source: https://learnopengl.com/PBR/Lighting

const float PI = 3.14159265359;

in vec3 viewDirectionNormalV;
in vec3 lightPositionV;
in vec3 vertexPositionV;
in vec2 texCoord;

uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
// uniform sampler2D u_Normal;
// uniform sampler2D u_Height;

uniform vec3 u_Emissive;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Alpha;
uniform float u_AmbientOcclusion;

uniform vec3 u_LightColour = vec3(1.0f, 1.0f, 1.0f);
// uniform vec3 ambientColour = { 0.1, 0.1, 0.2 };

out vec4 fragColour;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Normal Distribution Function (Trowbridge-Reitz GGX) - Approximates surface's microfacets that align to the halfway vector:
float DistrubutionGGX(const in vec3 normal, const in vec3 halfwayVector, const in float roughness)
{
	float roughnessPow2 = roughness * roughness;
	float normalDotHalfway = max(dot(normal, halfwayVector), 0.0);
	float normalDotHalfwayPow2 = normalDotHalfway * normalDotHalfway;
	float denominator = (normalDotHalfwayPow2 * (roughnessPow2 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;
	return roughnessPow2 / denominator;
}

// Fresnel Schlick - Ratio of light reflected based on the viewing angle:
vec3 FresnelSchlick(const in float cosTheta, vec3 baseReflectivity)
{
	return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - cosTheta, 5.0);
}

// Roughness Remapper (Used in Schlick-GGX):
float SchlickGGXRoughnessRemapperDirect(const in float roughness) { return pow((roughness + 1), 2.0) / 8.0; }
float SchlickGGXRoughnessRemapperIBL(const in float roughness) { return (roughness * roughness) / 2.0; }

// Schlick-GGX:
float GeometrySchlickGGX(float normalDotViewDirection, const in float remappedRoughness)
{
	return normalDotViewDirection / (normalDotViewDirection * (1.0 - remappedRoughness) + remappedRoughness);
}

// Schlick-GGX combined with Smith's method:
float GeometrySmith(const in vec3 normal, const in vec3 viewDirection, const in vec3 lightDirection, const in float remappedRoughness)
{
	float normalDotView = max(dot(normal, viewDirection), 0.0);
	float normalDotLight = max(dot(normal, lightDirection), 0.0);
	float ggx1 = GeometrySchlickGGX(normalDotView, remappedRoughness);
	float ggx2 = GeometrySchlickGGX(normalDotLight, remappedRoughness);
	return ggx1 * ggx2;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void main()
{
    vec4 resultTexture;
    resultTexture += texture(u_Diffuse, texCoord).rgba;
    resultTexture += texture2D(u_Specular, texCoord).rgba;
	// vec3 albedo = resultTexture.rgb;
	vec3 albedo = vec3(pow(resultTexture.r, 2.2f), pow(resultTexture.g, 2.2f), pow(resultTexture.b, 2.2f));
	// vec3 albedo = vec3(1.0f, 0.0f, 0.0f);
	

	vec3 lightDir = normalize( lightPositionV - vertexPositionV );
	vec3 normal = normalize( viewDirectionNormalV );
	vec3 eyeDir = normalize( -vertexPositionV );

	vec3 baseReflectivity = vec3(0.04);
	baseReflectivity = mix(baseReflectivity , albedo, u_Metallic);
	float remappedRoughness = SchlickGGXRoughnessRemapperIBL(u_Roughness);
	
	vec3 totalRadiance = vec3(0.0);
	for(int i = 0; i < 1; ++i)
	{
		// Calculate per-light radiance:
		vec3 L = normalize(lightPositionV - vertexPositionV);
		vec3 halfVec = normalize(lightDir + eyeDir);
		float distance = length(lightPositionV - vertexPositionV);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_LightColour * attenuation;

		// Cook-Torrence BRDF:
		float d = DistrubutionGGX(normal, halfVec, u_Roughness);
		vec3 f = FresnelSchlick(dot(normal, halfVec), baseReflectivity);
		float g = GeometrySmith(normal, eyeDir, lightDir, remappedRoughness);

		vec3 kS = f;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - u_Metallic;

		vec3 numerator = d * g * f;
		float denominator = 4.0f * max(dot(normal, eyeDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		// Add to outgoing radiance:
		float normalDotLight = max(dot(normal, lightDir), 0.0);
		totalRadiance += (kD * albedo / PI + specular) * radiance * normalDotLight;
	}

	vec3 ambient = vec3(0.03) * albedo * u_AmbientOcclusion;
	vec3 colour = ambient + totalRadiance;
	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0/2.2));

	fragColour = vec4( u_Emissive + colour, u_Alpha );
}
