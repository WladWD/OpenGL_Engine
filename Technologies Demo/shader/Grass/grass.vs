#version 440 core
////////////////////////////////////////////////////
#define OFFSET 10.0f
#define REPEAD ( 64.0f)
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 gWorld;
layout (location = 1)uniform mat4 gProjView;
layout (location = 2)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
void main()
{
	vec4 pos = gWorld * vec4(v_position, 1.0);
	vertex_out.w_pos = pos.xyz;
	//vertex_out.w_posdw = (gWorld * vec4(v_tex, 0.0f, 1.0)).xyy;//pos.xyz;
	vertex_out.w_eye_dir = pos.xyz - g_cam_pos;
	vertex_out.w_tex = (gWorld * vec4(v_tex, 0.0f, 1.0)).xy ;//* REPEAD;

	pos.y -= OFFSET;
	gl_Position = gProjView * pos;
	gl_Position.z = gl_Position.w;
}