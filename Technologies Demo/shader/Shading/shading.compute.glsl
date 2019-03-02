#version 440 core
#pragma optimize(off)
#pragma debug(on)
////////////////////////////////////////////////////
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
////////////////////////////////////////////////////
//in variable
////////////////////////////////////////////////////
#define MAX_POINT_LIGHTS 512
#define MAX_SPOT_LIGHTS 512
////////////////////////////////////////////////////
#define MAX_POINT_SHADOW_LIGHT 16
#define MAX_SPOT_SHADOW_LIGHT 16
////////////////////////////////////////////////////
////////////////////////////////////////////////////
//layout(binding = 0, r32f) readonly uniform image2D g_depth_tex;
//layout(binding = 0) uniform sampler2D depth_tex;
////////////////////////////////////////////////////
//out variable
////////////////////////////////////////////////////

layout(std430, binding = 0) buffer BufferObject {
	int v;
};

//layout(location = 5, binding = 5, rgba8) uniform image2D gtex;
layout(location = 2, binding = 3) uniform sampler2D gtex_di;
////////////////////////////////////////////////////
//groupshared

////////////////////////////////////////////////////
//compute method
////////////////////////////////////////////////////

// 
////////////////////////////////////////////////////
void main()
{
	//uvec3 globalCoord = gl_GlobalInvocationID;
	/*vec2 mdim = vec2(1000);//textureSize(depth_tex, 0);
	if (globalCoord.x < mdim.x && globalCoord.y < mdim.y)
	{
		ivec2       position = ivec2(gl_GlobalInvocationID.xy);
		vec2        screenNormalized = vec2(position) / mdim; // ctxSize is the size of the depth and color textures

		//imageStore(g_Framebuffer, ivec2(globalCoord.xy), imageLoad(g_depth_tex, ivec2(globalCoord.xy)));
		imageStore(g_Framebuffer, ivec2(globalCoord.xy), vec4(screenNormalized, 1.0f, 1.0f));//texture(depth_tex, screenNormalized));
		//texelFetch(texelFetch
		//imageStore(g_Framebuffer, ivec2(globalCoord.xy), texelFetch(g_depth_tex, ivec2(globalCoord.xy)));
	}*/
	//gl_GlobalInvocationID.xy
	v = 230555;
	//imageStore(gtex, ivec2(gl_GlobalInvocationID.xy), vec4(1.0f));//
	vec4 c = texture(gtex_di, vec2(gl_GlobalInvocationID.xy) * vec2(1.0/1800.0, 1.0/1000.0));//imageLoad(gtex, ivec2(gl_GlobalInvocationID.xy));
	v = int(c.x * 100.0f);
}