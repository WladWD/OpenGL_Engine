#version 440 core
//////////////////////////////////////////////////////////////////////
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec2 v_tex;
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
}vGS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//layout ( location = 0 ) uniform mat4 mProjView;
//////////////////////////////////////////////////////////////////////
void main()
{
	//gl_Position = mProjView * vec4(vWorldPos, 1.0f);
	vGS_IN.vWorldPos = v_position;
	vGS_IN.vTex = v_tex;
}
//////////////////////////////////////////////////////////////////////
