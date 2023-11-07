#type vertex
#version 430 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBiTangent;
layout(location = 4) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec4 u_WorldSpaceLightPos;
uniform vec4 u_CamPos = vec4(0.0, 2.0, 2.0, 1.0);

out vec2 texCoord;
out vec3 vertexNormal;
out vec3 fragPosition;
out vec3 lightPosition;
out vec3 viewPosition;
out mat3 TBN;

void main()
{
	texCoord = aTexCoord;
	vertexNormal = aNormal;

	// normal mapping: calculate tangent-bitangent-normal matrix.
	vec3 t = normalize(vec3(u_Model * vec4(aTangent, 0.0)));
	const vec3 n = normalize(vec3(u_Model * vec4(aNormal, 0.0)));
	t = normalize(t - dot(t, n) * n);
	const vec3 b = cross(n, t);
	TBN = mat3(t, b, n);

	lightPosition = vec3(u_WorldSpaceLightPos);
    viewPosition = vec3(u_CamPos);
    fragPosition = vec3(u_Model * aPos);

	gl_Position = u_Projection * u_View * u_Model * aPos;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#type fragment
#version 430 core
#define PI 3.14159265359

// Source: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// Source: https://learnopengl.com/PBR/Lighting

in vec2 texCoord;
in vec3 vertexNormal;

in vec3 fragPosition;
in vec3 lightPosition;
in vec3 viewPosition;
in mat3 TBN;

subroutine vec3 CalculateAlbedo();
subroutine uniform CalculateAlbedo AlbedoMode;

subroutine vec3 CalculateNormal();
subroutine uniform CalculateNormal NormalMode;

subroutine float CalculateRoughness();
subroutine uniform CalculateRoughness RoughnessMode;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_NormalMap;
uniform vec4 u_CamPos = vec4(0.0, 2.0, 2.0, 1.0);

uniform vec3 u_Albedo;
uniform float u_Roughness;
uniform float u_Metallic;
uniform vec3 u_Emissive;
uniform float u_AmbientOcclusion;
uniform float u_Alpha;

uniform vec3 u_LightColour = vec3(2.0, 2.0, 2.0);

out vec4 FragColour;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Normal Distribution Function (Trowbridge-Reitz GGX) - Approximates surface's microfacets that align to the halfway vector:
const float DistrubutionGGX(const in vec3 normal, const in vec3 halfwayVector, const in float roughness)
{
	float roughnessPow2 = roughness * roughness;
	float normalDotHalfway = max(dot(normal, halfwayVector), 0.0);
	float normalDotHalfwayPow2 = normalDotHalfway * normalDotHalfway;
	float denominator = (normalDotHalfwayPow2 * (roughnessPow2 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;
	return roughnessPow2 / denominator;
}

// Fresnel Schlick - Ratio of light reflected based on the viewing angle:
const vec3 FresnelSchlick(const in float cosTheta, const in vec3 baseReflectivity)
{
	return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - cosTheta, 5.0);
}

// roughness remapper (Used in Schlick-GGX):
const float SchlickGGXRoughnessRemapperDirect(const in float roughness) { return pow((roughness + 1), 2.0) / 8.0; }
const float SchlickGGXRoughnessRemapperIBL(const in float roughness) { return (roughness * roughness) / 2.0; }

// Schlick-GGX:
const float GeometrySchlickGGX(const in float normalDotViewDirection, const in float remappedRoughness)
{
	return normalDotViewDirection / (normalDotViewDirection * (1.0 - remappedRoughness) + remappedRoughness);
}

// Schlick-GGX combined with Smith's method:
const float GeometrySmith(const in vec3 normal, const in vec3 viewDirection, const in vec3 lightDirection, const in float remappedRoughness)
{
	float normalDotView = max(dot(normal, viewDirection), 0.0);
	float normalDotLight = max(dot(normal, lightDirection), 0.0);
	float ggx1 = GeometrySchlickGGX(normalDotView, remappedRoughness);
	float ggx2 = GeometrySchlickGGX(normalDotLight, remappedRoughness);
	return ggx1 * ggx2;
}

// subroutines: two per texture that can be loaded. 'AutoTexture' means the texture has been loaded,
// 'Manual' means the value is literal and set directly by the user.
subroutine(CalculateNormal) const vec3 CalculateNormalManual()
{
	return normalize(vertexNormal);
}

subroutine(CalculateNormal) const vec3 CalculateNormalAutoTexture()
{
	vec3 normal = texture(u_NormalMap, texCoord).rgb;
	normal = normal * 2.0 - 1.0;
	return normalize(TBN * normal);
}

subroutine(CalculateAlbedo) const vec3 CalculateAlbedoManual()
{
	return u_Albedo;
}

subroutine(CalculateAlbedo) const vec3 CalculateAlbedoAutoTexture()
{
	const vec3 albedo = texture(u_AlbedoMap, texCoord).rgb;
	return vec3(pow(albedo.r, 2.2f), pow(albedo.g, 2.2f), pow(albedo.b, 2.2f));
}

subroutine(CalculateRoughness) const float CalculateRoughnessManual()
{
	return u_Roughness;
}

subroutine(CalculateRoughness) const float CalculateRoughnessAutoTexture()
{
	return texture(u_RoughnessMap, texCoord).r;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void main()
{
	const vec3 normal = NormalMode();
	const float roughness = RoughnessMode();
	const float metallic = 1.0 - roughness;
	const vec3 albedo = mix(AlbedoMode(), vec3(0.0), metallic);

	const vec3 lightDir = normalize( lightPosition - fragPosition );
	const vec3 viewDir = normalize( vec3(u_CamPos) - fragPosition );

	const vec3 baseReflectivity = mix(vec3(0.04), albedo, metallic);
	// const float remappedRoughness = SchlickGGXRoughnessRemapperIBL(roughness);

	vec3 totalRadiance = vec3(0.0);
	for(int i = 0; i < 1; ++i)
	{
		// Calculate per-light radiance:
		const vec3 L = normalize(lightPosition - fragPosition);
		const vec3 halfVec = normalize(lightDir + viewDir);
		const float distance = length(lightPosition - fragPosition);
		const float attenuation = 1.0 / (distance * distance);
		const vec3 radiance = u_LightColour * attenuation;

		// Cook-Torrence BRDF:
		const float d = DistrubutionGGX(normal, halfVec, roughness);
		const vec3 f = FresnelSchlick(dot(normal, halfVec), baseReflectivity);
		const float g = GeometrySmith(normal, viewDir, lightDir, roughness);

		const vec3 kS = f;
		vec3 kD = vec3(1.0) - kS;
		kD = kS * ( 1.0 - metallic );

		const vec3 numerator = d * g * f;
		const float denominator = 4.0f * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
		const vec3 specular = numerator / denominator;

		// Add to outgoing radiance:
		const float normalDotLight = max(dot(normal, lightDir), 0.0);
		totalRadiance += (kD * albedo / PI + specular) * radiance * normalDotLight;
	}

	const vec3 ambient = vec3(0.03) * albedo * u_AmbientOcclusion;
	vec3 colour = ambient + totalRadiance;
	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0/2.2));
	FragColour = vec4( u_Emissive + colour, u_Alpha );
}