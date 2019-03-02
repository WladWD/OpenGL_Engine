#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
void main()
{
	vertex_out.w_pos = v_position;
	vertex_out.w_tex = v_tex;

	gl_Position = vec4(v_position, 1.0);
}
