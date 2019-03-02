#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3	v_pos;
layout (location = 1) in vec3	v_seed;
layout (location = 2) in vec3	v_speed;
//layout (location = 3) in vec3	v_data;
layout (location = 3) in float	v_random;
layout (location = 4) in uint	v_Type;
////////////////////////////////////////////////////
layout (location = 0)uniform float g_dt_s;
layout (location = 1)uniform vec3 g_TotalVel;
layout (location = 2)uniform vec3 g_EyePos;
layout (location = 3)uniform float g_HeightRange;
////////////////////////////////////////////////////
out vec3	vo_pos;
out vec3	vo_seed;
out vec3	vo_speed;
out float	vo_random;
out uint	vo_Type;
////////////////////////////////////////////////////
void main()
{
	vo_pos = v_pos + v_speed*g_dt_s + g_TotalVel.xyz;
	vo_seed = v_seed;
	vo_speed = v_speed;
	vo_random = v_random;	
	vo_Type = v_Type;			
     
    if(vo_pos.y <= (g_EyePos.y-g_HeightRange) )
    {
        float x = v_seed.x + g_EyePos.x;
        float z = v_seed.z + g_EyePos.z;
        float y = v_seed.y + g_EyePos.y;
		vo_Type = (vo_Type + 1) % 370;
        vo_pos = vec3(x,y,z);
    }
}