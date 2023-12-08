#type vertex
#version 430 core
// Source: https://learnopengl.com/PBR/IBL/Specular-IBL

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
#define PI 3.14159265359

in vec3 localPosition;

uniform samplerCube u_EnvironmentMap;
uniform float u_Roughness;

out vec4 fragColour;

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

float RadicalInverse_VdC(uint bits)
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
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates:
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector:
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

void main()
{
    // Shape of the distribution changes based on the viewing angle, so the viewing
    // angle is assumed to be zero (n = v = r), preventing lengthy reflections at 
    // grazing angles. 
	vec3 N = normalize(localPosition);
    vec3 R = N;
    vec3 V = R;

    const uint sampleCount = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColour = vec3(0.0);

    for(uint i = 0u; i < sampleCount; i++)
    {
        vec2 Xi = Hammersley(i, sampleCount);
        vec3 H = ImportanceSampleGGX(Xi, N, u_Roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            // Sample from the environment's mip level based on roughness/pdf:
            float D = DistrubutionGGX(N, H, u_Roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

            float resolution = 512.0; // resolution of source cubemap (per face).
            float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(sampleCount) * pdf + 0.0001);

            float mipLevel = u_Roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColour += textureLod(u_EnvironmentMap, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    prefilteredColour = prefilteredColour / totalWeight;
    fragColour = vec4(prefilteredColour, 1.0);
}