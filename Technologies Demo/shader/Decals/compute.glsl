#version 440 core
//#pragma debug(on)
/////////////////////////////////////////////////////////////////////////////
#define MAX_DECALE_COUNT 16
/////////////////////////////////////////////////////////////////////////////
#define NUM_THREAD_X 8
#define NUM_THREAD_Y 8
/////////////////////////////////////////////////////////////////////////////
layout(local_size_x = NUM_THREAD_X, local_size_y = NUM_THREAD_Y) in;
/////////////////////////////////////////////////////////////////////////////
layout(location = 0, binding = 0) uniform sampler2D gDepth;
layout(location = 1, binding = 1) uniform sampler2DArray gDecaleTextures;
layout(location = 2) uniform mat4 gDecaleMatrix[MAX_DECALE_COUNT];
layout(location = 18) uniform int gDecaleTextureID[MAX_DECALE_COUNT];
layout(location = 34) uniform int mRealDecaleCount;
layout(location = 35) uniform mat4 gProjViewInv;
/////////////////////////////////////////////////////////////////////////////
layout(location = 36, binding = 0, rgba8) uniform image2D gDiffuse;
/////////////////////////////////////////////////////////////////////////////

float ExtractDepth(ivec2 gInvocationID)
{
	float depth = texelFetch(gDepth, gInvocationID, 0).r;
	depth = 2.0f * depth - 1.0f;
	return depth;
}

vec3 ExtractPosition(ivec2 gInvocationID, ivec2 gTextureSize) 
{
	float gDepth = ExtractDepth(gInvocationID);
	vec2 mTexSize = vec2(gTextureSize);
	vec2 uv = vec2(gInvocationID) / mTexSize;
	uv = uv * 2.0f - 1.0f;
	vec4 gProjectedPos = vec4(uv, gDepth, 1.0f);
	vec4 gWorldPos = gProjViewInv * gProjectedPos;
	gWorldPos.xyz *= (1.0f / gWorldPos.w);
	return gWorldPos.xyz;
}

vec4 ComputeDecale(vec3 mWorldPos, int gDecaleID)
{
	vec4 mProj = gDecaleMatrix[gDecaleID] * vec4(mWorldPos, 1.0f);
	mProj.xyz *= (1.0f / mProj.w);
	mProj.xyz = mProj.xyz * 0.5 + 0.5;

	vec4 mDecaleColor = vec4(0.0f);

	if (mProj.x >= 0.0 && mProj.x <= 1.0 &&
		mProj.y >= 0.0 && mProj.y <= 1.0 && 
		mProj.z >= 0.0 && mProj.z <= 1.0)
		mDecaleColor += texture(gDecaleTextures, vec3(mProj.xy, gDecaleID));
	return mDecaleColor;
}

/////////////////////////////////////////////////////////////////////////////
void main() 
{
	ivec3 gGlobalInvocationID = ivec3(gl_GlobalInvocationID);
	ivec2 gTexDim = textureSize(gDepth, 0);
	if (gl_GlobalInvocationID.x < gTexDim.x && gl_GlobalInvocationID.y < gTexDim.y) 
	{
		vec3 gWorldPosition = ExtractPosition(gGlobalInvocationID.xy, gTexDim);
		
		vec4 mDiffuseColor = imageLoad(gDiffuse, gGlobalInvocationID.xy);

		for (int i = 0; i < mRealDecaleCount; ++i)
		{
			mDiffuseColor += ComputeDecale(gWorldPosition, i);
		}

		imageStore(gDiffuse, gGlobalInvocationID.xy, mDiffuseColor);
	}
}


