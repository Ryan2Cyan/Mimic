#type vertex
#version 430 core
// Source: https://learnopengl.com/PBR/IBL/Specular-IBL

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 textureCoordinates;

void main()
{
	textureCoordinates = aTexCoords;
	gl_Position = vec4(aPos, 1.0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#type fragment
#version 430 core
#define PI 3.14159265359

in vec2 textureCoordinates;
out vec2 fragColour;

const float RadicalInverse_VdC(uint bits)
{
 	bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

// Hammersley Sequence: Random low-discrepancy sequence based on the Quasi-Monte Carlo method, based on
// the Van Der Corput sequence mirroring a decimal binary representation around its decimal point.
const vec2 Hammersley(const uint i, const uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}


const vec3 ImportanceSampleGGX(const vec2 Xi, const vec3 N, const float roughness)
{
    const float a = roughness * roughness;
	
    const float phi = 2.0 * PI * Xi.x;
    const float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    const float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
    // from spherical coordinates to cartesian coordinates:
    const vec3 H = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
	
    // from tangent-space vector to world-space sample vector:
    const vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    const vec3 tangent = normalize(cross(up, N));
    const vec3 bitangent = cross(N, tangent);
	
    const vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

// Schlick-GGX: Calculates general value (based on roughness) for how much self-shadowing occurs on the
// surface's microfacets.
const float GeometrySchlickGGX(const in float NdotV, const in float k)
{
	return NdotV / (NdotV * (1.0 - k) + k);
}

// Smith: Geometric shadowing model, breaks the formula into two parts (light and view), using Schlick-GGX to
// calculate both sub-components.
const float GeometrySmith(const in vec3 N, const in vec3 V, const in vec3 L, const in float k)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);
	return ggx1 * ggx2;
}

vec2 IntegrateBRDF(float NdotV, float k)
{
    const vec3 V = vec3(sqrt(1.0 - NdotV * NdotV), 0.0, NdotV);
    const vec3 N = vec3(0.0, 0.0, 1.0);

    float A = 0.0;
    float B = 0.0;

    const uint sampleCount = 1024u;
    for(uint i = 0u; i < sampleCount; i++)
    {
        const vec2 Xi = Hammersley(i, sampleCount);
        const vec3 H = ImportanceSampleGGX(Xi, N, k);
        const vec3 L = normalize(2.0 * dot(V, H) * H - V);

        const float NdotL = max(L.z, 0.0);
        const float NdotH = max(H.z, 0.0);
        const float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            const float G = GeometrySmith(N, V, L, k);
            const float G_Vis = (G * VdotH) / (NdotH * NdotV);
            const float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    const float sampleCountF = float(sampleCount);
    A /= sampleCountF;
    B /= sampleCountF;
    return vec2(A, B);
}

void main()
{
	fragColour = IntegrateBRDF(textureCoordinates.x, textureCoordinates.y);
}