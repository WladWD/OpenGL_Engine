#version 440 core

in struct Vertex {
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 tex0;
} vertex;

const int kernelSize = 64;
const float radius = 0.5f;
const float depthBias = 0.025f;

layout(location = 0) out vec4 framebufferColor;

layout(location = 0) uniform mat4 mProj;
layout(location = 1) uniform mat4 mInvProj;

layout(location = 2) uniform mat4 mView;
layout(location = 3) uniform mat4 mInvView;

layout(location = 4) uniform mat4 mProjView;
layout(location = 5) uniform mat4 mInvProjView;

layout(location = 6) uniform mat4 mWorld;
layout(location = 7) uniform mat4 mInvWorld;

layout(location = 8) uniform mat4 mProjViewWorld;
layout(location = 9) uniform mat4 mInvProjViewWorld;

layout(location = 10, binding = 0) uniform sampler2D gDepth;
layout(location = 11, binding = 1) uniform sampler2D gNormal;
layout(location = 12, binding = 2) uniform sampler2D gDiffuse;
layout(location = 13, binding = 3) uniform sampler2D gSpecular;
layout(location = 14, binding = 4) uniform sampler2D gNoise;

layout(location = 15) uniform vec2 noiseScale; 
layout(location = 16) uniform vec4 randomVector[kernelSize];

vec3 getViewPosition(in vec2 texCoord) {
	float depth = texture(gDepth, texCoord).r;
	vec3 projPosition = vec3(texCoord, depth) * 2.0f - 1.0f;
	vec4 viewPosition4 = mInvProj * vec4(projPosition, 1.0f);
	return viewPosition4.xyz;
}

void main() {
	mat4 invTranfView = transpose(inverse(mView));
	vec3 viewPosition = getViewPosition(vertex.tex0);
	vec3 normal = vec3(invTranfView * vec4(texture(gNormal, vertex.tex0).xyz, 0.0f));
	vec3 randomVec = normalize(texture(gNoise, vertex.tex0 * noiseScale).xyz);

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0f;
	for (int i = 0; i < kernelSize; ++i) {
		vec3 sampleVec = randomVector[i].xyz;
		vec3 samplePosition = viewPosition + TBN * sampleVec * radius;
		vec4 sampleNDC = vec4(samplePosition, 1.0f);
		sampleNDC = mProj * sampleNDC;
		sampleNDC *= 1.0f / sampleNDC.w;
		sampleNDC = sampleNDC * 0.5f + 0.5f;

		float sampleDepth = getViewPosition(sampleNDC.st).z;
		float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(sampleDepth - viewPosition.z));
		occlusion += (sampleDepth >= samplePosition.z + depthBias ? 1.0f : 0.0f) * rangeCheck;
	}
	occlusion = 1.0f - (occlusion / float(kernelSize));

	framebufferColor = vec4(occlusion);
}
