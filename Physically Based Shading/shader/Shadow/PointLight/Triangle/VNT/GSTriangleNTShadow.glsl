#version 440 core
//////////////////////////////////////////////////////////////////////
#define SHADOW_MAP_MAX_COUNT 16
#define CUBE_MAP_FACE_COUNT 6
#define MATRIX_MAX_COUNT SHADOW_MAP_MAX_COUNT * CUBE_MAP_FACE_COUNT
//////////////////////////////////////////////////////////////////////
layout(triangles, invocations = CUBE_MAP_FACE_COUNT) in;
layout(triangle_strip, max_vertices = CUBE_MAP_FACE_COUNT * 3) out;
//////////////////////////////////////////////////////////////////////
in struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
	int mInstanceID;
}vGS_IN[];
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct PS_IN_BLOCK 
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout ( location = 1 ) uniform mat4 mProjView[MATRIX_MAX_COUNT];
//layout ( location = MATRIX_MAX_COUNT) uniform uint uShadowMapCount;
//////////////////////////////////////////////////////////////////////
void main()
{
	//gl_InstanceID;
	//gl_InvocationID;
	//gl_Layer;
	//mInstanceID;
	//int mId = gl_InstanceID * CUBE_MAP_FACE_COUNT + gl_InvocationID;

	for (int j = 0; j < gl_in.length(); j++)
	{
		int mId = vGS_IN[j].mInstanceID * CUBE_MAP_FACE_COUNT + gl_InvocationID;

		gl_Layer = mId;
		gl_Position = mProjView[mId] * vec4(vGS_IN[j].vWorldPos, 1.0f);
		vPS_IN.vTex = vGS_IN[j].vTex;
		EmitVertex();
	}
	EndPrimitive();
}
//////////////////////////////////////////////////////////////////////
