#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 m_tn;
layout (location = 3) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 g_proj_view;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
void main()
{
	vertex_out.w_pos = v_position;
	vertex_out.w_normal = v_normal;
	vertex_out.w_tangent = m_tn;
	vertex_out.w_tex = v_tex;
	vertex_out.w_eye_dir = v_position - g_cam_pos;

	gl_Position = g_proj_view * vec4(v_position, 1.0);
}