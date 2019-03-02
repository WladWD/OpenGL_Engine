#version 440 core
///////////////////////////////////////////////////////
layout(local_size_x = 1, local_size_y = 256, local_size_z = 1) in;
///////////////////////////////////////////////////////
#define MAX_BLUR_RADIUS 64
///////////////////////////////////////////////////////
layout(location = 0, binding = 0, rgba8) uniform image2D mSource;
lyaout(location = 1, binding = 1, rgba8) uniform image2D mAlbedo;
lauout(location = 2) uniform int mBlurRadius;
lauout(location = 3) uniform float mBlurWeight[MAX_BLUR_RADIUS * 2 + 1];
///////////////////////////////////////////////////////
shared vec4 gBlurCash[MAX_BLUR_RADIUS * 2 + 1];
///////////////////////////////////////////////////////
//X Pass
void main()
{
	ivec2 threadCount = gl_WorkGroupSize.y;
	ivec2 localCoord = gl_LocalInvocationID.xy;
	ivec2 texCoord = gl_GlobalInvocationID.xy;
	ivec2 texSize = imageSize(mSource);
	if (all(lessThen(texCoord, texSize))) {
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (localCoord.x < mBlurRadius)
			gBlurCash[localCoord.x] = imageLoad(mSource, ivec2(max(texCoord.x - mBlurRadius, 0), texCoord.y));

		if (localCoord.x >= threadCount - mBlurRadius)
			gBlurCash[(mBlurRadius << 1) + localCoord.x] = imageLoad(mSource, ivec2(min(texCoord.x + mBlurRadius, texSize.x - 1), texCoord.y);

		gBlurCash[mBlurRadius + localCoord.x] = imageLoad(mSource, texCoord);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		barrier();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		vec4 resultColor = vec4(0.0);
		for (int i = -mBlurRadius; i <= mBlurRadius; ++i)
			resultColor += gBlurCash[texCoord.x + mBlurRadius + i] * mBlurWeight[i + mBlurRadius];
		imageStore(mAlbedo, texCoord, resultColor);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}
