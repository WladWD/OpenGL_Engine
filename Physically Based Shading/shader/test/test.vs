#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 out_v_pos;
	vec3 out_v_normal;
	vec2 out_v_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 proj_view_world;
////////////////////////////////////////////////////
void main()
{
	vec4 pos = vec4(v_position, 1.0);
	vertex_out.out_v_pos = v_position;
	vertex_out.out_v_normal = v_normal;
	vertex_out.out_v_tex = v_tex;

	gl_Position = proj_view_world * pos;
}