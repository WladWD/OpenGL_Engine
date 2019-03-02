#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct FS_INPUT
{
	vec3 w_pos;
	vec2 w_tex0;
	vec3 w_tex1;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view_world;
////////////////////////////////////////////////////
void main()
{
	fs_input.w_pos = v_position;
	fs_input.w_tex0 = v_tex;
	fs_input.w_tex1 = v_position;//-v_position

	gl_Position = g_proj_view_world * vec4(v_position, 1.0);
	gl_Position.z = gl_Position.w;
}
