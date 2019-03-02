#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct FS_INPUT
{
	vec3 w_pos;
	vec2 w_tex;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view_world;
layout (location = 1) uniform mat4 g_world;
layout (location = 2) uniform float g_alpha;
////////////////////////////////////////////////////
void main()
{
	vec4 mpos = vec4(v_position, 1.0f);
	fs_input.w_pos = (g_world * mpos).xyz;
	fs_input.w_tex = v_tex;

	gl_Position = g_proj_view_world * mpos;
	gl_Position.z = gl_Position.w;
}
