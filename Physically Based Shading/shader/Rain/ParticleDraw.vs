#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3	v_pos;
layout (location = 1) in vec3	v_seed;
layout (location = 2) in vec3	v_speed;
layout (location = 3) in float	v_random;
layout (location = 4) in uint	v_Type;
////////////////////////////////////////////////////
out struct GSInput
{
	vec3	v_pos;
	vec3	v_seed;
	vec3	v_speed;
	float	v_random;
	uint	v_Type;
} gs_in;
////////////////////////////////////////////////////
void main()
{
	gs_in.v_pos = v_pos;
	gs_in.v_seed = v_seed;
	gs_in.v_speed = v_speed;
	gs_in.v_random = v_random;
	gs_in.v_Type = v_Type;
}
