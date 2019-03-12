#version 440 core

const int kernelSize = 8;
const int kernelDim = kernelSize - 1;

in struct Vertex {
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 tex0;
} vertex;

layout(location = 0) out vec4 framebufferColor;

layout(location = 10, binding = 0) uniform sampler2D gTexture;
layout(location = 11) uniform float mipBias;
layout(location = 12) uniform vec2 texOffset;
layout(location = 13) uniform float gaussian[kernelSize]; 

void main() {
	vec2 tex = vertex.tex0 - texOffset * float(kernelDim);
	vec4 blurColor = vec4(0.0);
	for (int i = -kernelDim; i <= kernelDim; ++i) {
		blurColor += gaussian[abs(i)] * texture2D(gTexture, tex, mipBias);
		tex += texOffset;
	}
	//blurColor = texture2D(gTexture, vertex.tex0, mipBias);

	framebufferColor = blurColor;
}
