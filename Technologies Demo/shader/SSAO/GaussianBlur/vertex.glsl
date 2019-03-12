#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 tex0;

out struct Vertex {
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 tex0;
} vertex;

//layout(location = 0) uniform mat4 mProj;
//layout(location = 1) uniform mat4 mInvProj;
//
//layout(location = 2) uniform mat4 mView;
//layout(location = 3) uniform mat4 mInvView;
//
//layout(location = 4) uniform mat4 mProjView;
//layout(location = 5) uniform mat4 mInvProjView;
//
//layout(location = 6) uniform mat4 mWorld;
//layout(location = 7) uniform mat4 mInvWorld;
//
//layout(location = 8) uniform mat4 mProjViewWorld;
//layout(location = 9) uniform mat4 mInvProjViewWorld;

void main() {
	vec4 hPosition = vec4(position, 1.0f);

	vertex.position = position;
	vertex.normal = normal;
	vertex.tangent = tangent;
	vertex.tex0 = tex0;

	gl_Position = hPosition;
}


