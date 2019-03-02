#version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
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
	vertex_out.w_tex = v_tex;
	vertex_out.w_eye_dir = v_position - g_cam_pos;

	gl_Position = g_proj_view * vec4(v_position, 1.0);
}
/*
struct VSParticleIn
{   
    float3 pos              : POSITION;         //position of the particle
    float3 seed             : SEED;
    float3 speed            : SPEED;
    float random            : RAND;
    uint   Type             : TYPE;             //particle type
};


VSParticleIn VSAdvanceRain(VSParticleIn input)
{
     if(moveParticles)
     {
         //move forward
         input.pos.xyz += input.speed.xyz/g_FrameRate + g_TotalVel.xyz;

         //if the particle is outside the bounds, move it to random position near the eye         
         if(input.pos.y <=  g_eyePos.y-g_heightRange )
         {
            float x = input.seed.x + g_eyePos.x;
            float z = input.seed.z + g_eyePos.z;
            float y = input.seed.y + g_eyePos.y;
            input.pos = float3(x,y,z);
         }
    }

    return input;
    
}*/