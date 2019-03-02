#version 440 core
////////////////////////////////////////////////////
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
////////////////////////////////////////////////////
in struct GSInput
{
	vec3	v_pos;
	vec3	v_seed;
	vec3	v_speed;
	float	v_random;
	uint	v_Type;
} gs_in[];
////////////////////////////////////////////////////
out vec2 ps_in_tex;
out	vec3 ps_in_pos;
out	flat uint ps_in_tex_id;
////////////////////////////////////////////////////
layout (location = 0)uniform float g_SpriteSize; //0.8;
layout (location = 1)uniform float g_dt_s;
layout (location = 2)uniform vec3 g_TotalVel;
layout (location = 3)uniform vec3 g_eyePos;
layout (location = 4)uniform mat4 g_mProjViewWorld;
layout (location = 5)uniform vec4 g_SunLight;
layout (location = 6)uniform vec4 g_MoonLight;
////////////////////////////////////////////////////
void GenRainSpriteVertices(vec3 worldPos, vec3 velVec, vec3 eyePos, out vec3 outPos[4])
{
    float height = g_SpriteSize/2.0;
    float width = height/10.0;

	width *= 3.0f;//4.0f;
	height *= 3.0f;//4.0f;

    velVec = normalize(velVec);
    vec3 eyeVec = eyePos - worldPos;
    vec3 eyeOnVelVecPlane = eyePos - ((dot(eyeVec, velVec)) * velVec);
    vec3 projectedEyeVec = eyeOnVelVecPlane - worldPos;
    vec3 sideVec = normalize(cross(projectedEyeVec, velVec));
    
    outPos[0] =  worldPos - (sideVec * 0.5*width);
    outPos[1] = outPos[0] + (sideVec * width);
    outPos[2] = outPos[0] + (velVec * height);
    outPos[3] = outPos[2] + (sideVec * width );
}
////////////////////////////////////////////////////

////////////////////////////////////////////////////
void main()
{
    vec2 g_texcoords[4] = 
    { 
        vec2(0,1), 
        vec2(1,1),
        vec2(0,0),
        vec2(1,0),
    };

	vec3 pos[4];
    GenRainSpriteVertices(gs_in[0].v_pos, gs_in[0].v_speed*g_dt_s + g_TotalVel, g_eyePos, pos);
    
	gl_Position = g_mProjViewWorld * vec4(pos[0], 1.0f);
	ps_in_tex = g_texcoords[0];
	ps_in_pos = pos[0];
	ps_in_tex_id = int(gs_in[0].v_Type);
    EmitVertex();
        
    gl_Position = g_mProjViewWorld * vec4(pos[1], 1.0f);
    ps_in_tex = g_texcoords[1];
	ps_in_pos = pos[1];
	ps_in_tex_id = int(gs_in[0].v_Type);
    EmitVertex();
        
    gl_Position = g_mProjViewWorld * vec4(pos[2], 1.0f);
    ps_in_tex = g_texcoords[2];
	ps_in_pos = pos[2];
	ps_in_tex_id = int(gs_in[0].v_Type);
    EmitVertex();
                
    gl_Position = g_mProjViewWorld * vec4(pos[3], 1.0f);
    ps_in_tex = g_texcoords[3];
	ps_in_pos = pos[3];
	ps_in_tex_id = int(gs_in[0].v_Type);
    EmitVertex();
        
    EndPrimitive();
}
