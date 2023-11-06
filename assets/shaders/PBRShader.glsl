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

// invariant keyword ensures that multiple program's output will be the same:
// out vec3 viewDirectionNormalV;
// out vec3 lightPositionV;
// out vec3 vertexPositionV;
out vec2 texCoord;

out vec3 tangentFragPosition;
out vec3 tangentLightPosition;
out vec3 tangentViewPosition;
out mat3 TBN;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * aPos;

	// vertexPositionV = vec3(u_View * u_Model * aPos);
	// lightPositionV = vec3(u_View * u_WorldSpaceLightPos);
	// viewDirectionNormalV = mat3(u_View * u_Model) * aNormal;
	texCoord = aTexCoord;

	// fragPosition - vec3(u_Model * aPos);

	vec3 t = normalize(vec3(u_Model * vec4(aTangent, 0.0)));
	const vec3 n = normalize(vec3(u_Model * vec4(aNormal, 0.0)));
	t = normalize(t - dot(t, n) * n);
	const vec3 b = cross(n, t);

	TBN = transpose(mat3(t, b, n));
	tangentLightPosition = TBN * vec3(u_WorldSpaceLightPos);
    tangentViewPosition  = TBN * vec3(u_CamPos);
    tangentFragPosition  = TBN * vec3(u_Model * aPos);
}




#type fragment
#version 430 core
#pragma debug(on)
#define PI 3.14159265359

// Source: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// Source: https://learnopengl.com/PBR/Lighting

// in vec3 viewDirectionNormalV;
// in vec3 lightPositionV;
// in vec3 vertexPositionV;
in vec2 texCoord;

in vec3 tangentFragPosition;
in vec3 tangentLightPosition;
in vec3 tangentViewPosition;
in mat3 TBN;

subroutine vec3 CalculatePBRColour();
subroutine uniform CalculatePBRColour PBRMode;
vec3 CalculatePBRColourManual();
vec3 CalculatePBRColourAutoTexture();

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_NormalMap;
uniform vec4 u_CamPos = vec4(0.0, 2.0, 2.0, 1.0);

uniform vec3 u_Emissive;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Alpha;
uniform float u_AmbientOcclusion;

uniform vec3 u_LightColour = vec3(2.0, 2.0, 2.0);
// uniform vec3 ambientColour = { 0.1, 0.1, 0.2 };

out vec4 FragColour;

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
vec3 FresnelSchlick(const in float cosTheta, const in vec3 baseReflectivity)
{
	return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - cosTheta, 5.0);
}

// roughness remapper (Used in Schlick-GGX):
float SchlickGGXRoughnessRemapperDirect(const in float roughness) { return pow((roughness + 1), 2.0) / 8.0; }
float SchlickGGXRoughnessRemapperIBL(const in float roughness) { return (roughness * roughness) / 2.0; }

// Schlick-GGX:
float GeometrySchlickGGX(const in float normalDotViewDirection, const in float remappedRoughness)
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

// Manual Mode: excludes roughness & metallic maps. uses the values defined by the user:
subroutine(CalculatePBRColour) vec3 CalculatePBRColourManual()
{
	const vec3 diffuse = texture(u_AlbedoMap, texCoord).rgb;
	const vec3 albedo = vec3(pow(diffuse.r, 2.2f), pow(diffuse.g, 2.2f), pow(diffuse.b, 2.2f));

	const vec3 lightDir = TBN * normalize( tangentLightPosition - tangentFragPosition );
	const vec3 viewDir = TBN * normalize( vec3(u_CamPos) - tangentFragPosition );

	// const vec3 normal = normalize( viewDirectionNormalV );
	vec3 normal = texture(u_NormalMap, texCoord).rgb;
	normal = normal * 2.0 - 1.0;

	vec3 baseReflectivity = vec3(0.04);
	baseReflectivity = mix(baseReflectivity , albedo, u_Metallic);
	const float remappedRoughness = SchlickGGXRoughnessRemapperIBL(u_Roughness);
	
	vec3 totalRadiance = vec3(0.0);
	for(int i = 0; i < 1; ++i)
	{
		// Calculate per-light radiance:
		const vec3 L = normalize(tangentLightPosition - tangentFragPosition);
		const vec3 halfVec = normalize(lightDir + viewDir);
		const float distance = length(tangentLightPosition - tangentFragPosition);
		const float attenuation = 1.0 / (distance * distance);
		const vec3 radiance = u_LightColour * attenuation;

		// Cook-Torrence BRDF:
		const float d = DistrubutionGGX(normal, halfVec, u_Roughness);
		const vec3 f = FresnelSchlick(dot(normal, halfVec), baseReflectivity);
		const float g = GeometrySmith(normal, viewDir, lightDir, remappedRoughness);

		const vec3 kS = f;
		const vec3 kD = (vec3(1.0) - kS) * (1.0 - u_Metallic);

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
	return colour;
}

// Auto Texture Mode: roughness (and metalness) are loaded from a texture,
// albedo (Kd_map), roughness/metalness (Ks_map), Normal (map_Bump).
subroutine(CalculatePBRColour)
vec3 CalculatePBRColourAutoTexture()
{

	vec3 normal = texture(u_NormalMap, texCoord).rgb;
	normal = normal * 2.0 - 1.0;

	const vec3 lightDir = TBN * normalize( tangentLightPosition - tangentFragPosition );
	const vec3 viewDir = TBN * normalize( vec3(u_CamPos) - tangentFragPosition );

	const vec3 diffuse = texture(u_AlbedoMap, texCoord).rgb;
	vec3 albedo = vec3(pow(diffuse.r, 2.2f), pow(diffuse.g, 2.2f), pow(diffuse.b, 2.2f));

	const float roughness = texture(u_RoughnessMap, texCoord).r;
	// const float remappedRoughness = SchlickGGXRoughnessRemapperIBL(roughness);
	const float metallic = 1.0 - roughness;

	const vec3 baseReflectivity = mix(vec3(0.04), albedo, metallic);
	albedo = mix(albedo, vec3(0), metallic);

	vec3 totalRadiance = vec3(0.0);
	for(int i = 0; i < 1; ++i)
	{
		// Calculate per-light radiance:
		const vec3 L = normalize(tangentLightPosition - tangentFragPosition);
		const vec3 halfVec = normalize(lightDir + viewDir);
		const float distance = length(tangentLightPosition - tangentFragPosition);
		const float attenuation = 1.0 / (distance * distance);
		const vec3 radiance = u_LightColour * attenuation;

		// Cook-Torrence BRDF:
		const float d = DistrubutionGGX(normal, halfVec, roughness);
		const vec3 f = FresnelSchlick(dot(normal, halfVec), baseReflectivity);
		const float g = GeometrySmith(normal, viewDir, lightDir, roughness);

		const vec3 kS = f;
		//vec3 kD = vec3(1.0) - kS;
		const vec3 kD = kS * ( 1.0 - metallic );

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
	return colour;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void main()
{
	const vec3 colour = PBRMode();
	FragColour = vec4( u_Emissive + colour, u_Alpha );
}
