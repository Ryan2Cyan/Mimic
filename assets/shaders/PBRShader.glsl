#type vertex
#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBiTangent;
layout(location = 4) in vec2 aTexCoord;

uniform vec4 u_ViewPosition;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
// uniform mat3 u_NormalMatrix;

uniform mat4 u_DirectLightMatrices[25];
uniform int u_DirectLightsCount;
// uniform mat4 u_PointLightMatrices[20];

out vec2 texCoord;
// out vec3 vertexNormal;
out vec3 fragPosition;
out vec3 viewDirectionNormal;
out mat3 TBN;
out vec4 directionalLightSpacePositions[25];

void main()
{
	texCoord = aTexCoord;

	// normal mapping: calculate tangent-bitangent-normal matrix.
	vec3 t = normalize(vec3(u_Model * vec4(aTangent, 0.0)));
	const vec3 n = normalize(vec3(u_Model * vec4(aNormal, 0.0)));
	t = normalize(t - dot(t, n) * n);
	const vec3 b = cross(n, t);
	TBN = mat3(t, b, n);
	
	const vec4 aPosVec4 = vec4(aPos, 1.0);
        fragPosition = vec3(u_Model * aPosVec4);
	viewDirectionNormal = transpose(inverse(mat3(u_Model))) * aNormal;
	
	// calculate light space positions for shadow maps:
	for(int i = 0; i < u_DirectLightsCount; i++)
	{
		directionalLightSpacePositions[i] = u_DirectLightMatrices[i] * u_Model * aPosVec4;
	}	

	gl_Position = u_Projection * u_View * u_Model * aPosVec4;
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
// in vec3 vertexNormal;

in vec3 fragPosition;
in vec3 viewDirectionNormal;
in mat3 TBN;
in vec4 directionalLightSpacePositions[25];

// subroutine definitions:
subroutine vec3 CalculateAlbedo();
subroutine uniform CalculateAlbedo AlbedoMode;

subroutine vec3 CalculateNormal();
subroutine uniform CalculateNormal NormalMode;

subroutine float CalculateRoughness();
subroutine uniform CalculateRoughness RoughnessMode;

subroutine float CalculateMetallic();
subroutine uniform CalculateMetallic MetallicMode;

uniform vec3 u_CameraPosition;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicMap;
uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BRDFLookupTexture;
uniform sampler2D u_DirectShadowMaps[25];

uniform vec3 u_Albedo;
uniform float u_Roughness;
uniform float u_Metallic;
uniform vec3 u_Emissive;
uniform float u_AmbientOcclusion;
uniform float u_Alpha;

struct DirectLight
{
	vec4 colour;
	vec3 position;
	vec3 direction;
};
uniform DirectLight u_DirectLights[25];
uniform int u_DirectLightsCount;
struct PointLight
{
	vec4 colour;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};
uniform PointLight u_PointLights[25];
uniform int u_PointLightsCount;

out vec4 FragColour;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// calculate directional shadow map:
const float ShadowCalculation(const vec4 lightSpacePos, const vec3 lightDir, const sampler2D shadowMap, const vec3 normal)
{
	vec3 projectedCoords = (lightSpacePos.xyz / lightSpacePos.w) * 0.5 + 0.5;
	if(projectedCoords.z > 1.0) return 0.0;

	const float closestDepth = texture(shadowMap, projectedCoords.xy).r;
	const float currentDepth = projectedCoords.z;
	const float shadowBias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	// percentage-closer filtering to create the illusion of higher resolution shadows:
	float shadow = 0.0;
	const vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			const float pcfDepth = texture(shadowMap, projectedCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - shadowBias > pcfDepth ? 1.0 : 0.0;
		}
	}
	return shadow /= 9.0;
}

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

const vec3 FresnelSchlickRoughness(const float cosTheta, const vec3 baseReflectivity, const float roughness)
{
    return baseReflectivity + (max(vec3(1.0 - roughness), baseReflectivity) - baseReflectivity) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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
	return viewDirectionNormal;
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

subroutine(CalculateMetallic) const float CalculateMetallicManual()
{
	return u_Metallic;
}

subroutine(CalculateMetallic) const float CalculateMetallicAutoTexture()
{
	return texture(u_MetallicMap, texCoord).r;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void main()
{
    // Surface Normal: Direction the surface (this fragment is on) is facing.
	const vec3 N = NormalMode();
	const vec3 viewDir = normalize( u_CameraPosition - fragPosition );
	const vec3 reflectionDir = reflect(-viewDir, N);

	const float roughness = RoughnessMode();
	const float metallic = MetallicMode();
	const vec3 albedo = mix(AlbedoMode(), vec3(0.0), metallic);
	const vec3 baseReflectivity = mix(vec3(0.04), albedo, metallic);
	
	// Irradiance: The sum of radiances for each light source, scaled by a BRDF and the incident angle (dot of 
	// normal and light direction) for this fragment point.
	vec3 Lo = vec3(0.0);
	
	// Sum radiance for all direct lights:
	for(int i = 0; i < u_DirectLightsCount; ++i)
	{
		const vec3 lightPosition = u_DirectLights[i].position;

		// Light Direction.
		const vec3 Wi = u_DirectLights[i].direction;

		// Halfway Vector: A unit vector halfway between the view direction and the light direction. The closer
		// this vector aligns with the surface normal, the higher the specular contribution.
		const vec3 H = normalize(viewDir + Wi);
		
		// Radiance: The spectral radiance of the incoming light ray (represented as an RGB value).
		const vec3 Li = u_DirectLights[i].colour.rgb;

		// Cook-Torrence BRDF:
		const float d = DistrubutionGGX(N, H, roughness);
		const vec3 f = FresnelSchlick(dot(N, H), baseReflectivity);
		const float g = GeometrySmith(N, viewDir, Wi, roughness);

		const vec3 kS = f;
		// vec3 kD = vec3(1.0) - kS;
		vec3 kD = kS * ( 1.0 - metallic );

		const vec3 numerator = d * g * f;
		const float denominator = 4.0f * max(dot(N, viewDir), 0.0) * max(dot(N, Wi), 0.0) + 0.0001;
		const vec3 specular = numerator / denominator;
		
		// calculate shadows:
		float shadow = ShadowCalculation(directionalLightSpacePositions[i], Wi, u_DirectShadowMaps[i], N);

		// Incident angle: Used in Lambertian Diffusion, where the greater the cosine angle between light and surface 
		// normal, the less impact the light ray will have on the irradiance.
		const float cosTheta = max(dot(N, Wi), 0.0);

		// Reflectance Equation: Factoring in Lambertian Diffusion (diffuse), the Cook-Torrance BRDF (specular) and the 
		// light's radiance.
		Lo += (kD * albedo / PI + specular) * Li * cosTheta * (1.0 - shadow);
	}

	// sum radiance for all point lights:
	for(int i = 0; i < u_PointLightsCount; ++i)
	{
		const vec3 lightPosition = u_PointLights[i].position;
		const vec3 lightDirection = normalize(lightPosition - fragPosition);

		const vec3 halfVec = normalize(viewDir + lightDirection);
		const float distance = length(lightPosition - fragPosition);
		const float attenuation = 1.0 / (u_PointLights[i].constant + u_PointLights[i].linear * distance + u_PointLights[i].quadratic * (distance * distance));
		const vec3 radiance = vec3(u_PointLights[i].colour) * attenuation;

		// Cook-Torrence BRDF:
		const float d = DistrubutionGGX(N, halfVec, roughness);
		const vec3 f = FresnelSchlick(dot(N, halfVec), baseReflectivity);
		const float g = GeometrySmith(N, viewDir, lightDirection, roughness);

		const vec3 kS = f;
		// vec3 kD = vec3(1.0) - kS;
		vec3 kD = kS * ( 1.0 - metallic );

		const vec3 numerator = d * g * f;
		const float denominator = 4.0f * max(dot(N, viewDir), 0.0) * max(dot(N, lightDirection), 0.0) + 0.0001;
		const vec3 specular = numerator / denominator;

		// Add to outgoing radiance:
		const float normalDotLight = max(dot(N, lightDirection), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * normalDotLight;
	}

	// calculate ambience:
	// const vec3 ambient = vec3(0.03) * albedo * u_AmbientOcclusion;
	const vec3 fresnel = FresnelSchlickRoughness(max(dot(N, viewDir), 0.0), baseReflectivity, roughness);
	const vec3 kS = fresnel;
	const vec3 kD = 1.0 - kS;
	const vec3 irradiance = texture(u_IrradianceMap, N).rgb;
	const vec3 diffuse = irradiance * albedo;

	const float maxReflectionLOD = 4.0;
	const vec3 prefilteredColour = textureLod(u_PrefilterMap, reflectionDir, roughness * maxReflectionLOD).rgb;
	const vec2 brdf = texture(u_BRDFLookupTexture, vec2(max(dot(N, viewDir), 0.0), roughness)).rg;
	const vec3 specular = prefilteredColour * (fresnel * brdf.x + brdf.y);

	const vec3 ambient = (kD * diffuse + specular) * u_AmbientOcclusion;

	vec3 colour = ambient + Lo;
	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0 / 2.2));
	FragColour = vec4( u_Emissive + colour, u_Alpha );
}