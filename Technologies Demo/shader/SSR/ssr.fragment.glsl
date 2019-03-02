#version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in; 
////////////////////////////////////////////////////
#define SSR_ITTERATION 64
////////////////////////////////////////////////////
in vec2 gTex;
////////////////////////////////////////////////////
layout(location = 0) uniform vec3 gCamPos;
layout(location = 1) uniform vec3 gCamDir;
////////////////////////////////////////////////////
layout(location = 2, binding = 0) uniform sampler2D gSourceDepth;
layout(location = 3, binding = 1) uniform sampler2D gSourceImage;
////////////////////////////////////////////////////
layout(location = 4) uniform mat4 gProjView_Source;
layout(location = 5) uniform mat4 gProjViewInv_Source;
////////////////////////////////////////////////////
layout(location = 6, binding = 2) uniform sampler2D gDepth;
layout(location = 7, binding = 3) uniform sampler2D gNormal;
layout(location = 8, binding = 4) uniform sampler2D gDiffuse;
layout(location = 9, binding = 5) uniform sampler2D gSpecular;
////////////////////////////////////////////////////
layout(location = 10) uniform mat4 gProjView;
layout(location = 11) uniform mat4 gProjViewInv;
////////////////////////////////////////////////////
layout(location = 0) out vec4 gResColor;
////////////////////////////////////////////////////
vec3 RestoteWorldPosition( vec2 mTex ) 
{
	ivec2 iSize = textureSize(gDepth, 0);
	ivec2 iPosition = ivec2(vec2(iSize) * gTex);
	float fDepth = texture(gDepth, mTex).r;//texelFetch(gDepth, iPosition, 0).r;
	vec2 mXYPos = mTex;

	vec3 mWPos = vec3(mXYPos, fDepth);
	mWPos = 2.0f * mWPos - 1.0f;

	vec4 mPos = gProjViewInv * vec4(mWPos, 1.0f);
	mWPos.xyz = mPos.xyz * (1.0f / mPos.w);
	return mWPos.xyz;
}
////////////////////////////////////////////////////
vec3 RestoteWorldPositionSource(vec2 mTex )
{
	ivec2 iSize = textureSize(gSourceDepth, 0);
	ivec2 iPosition = ivec2(vec2(iSize-ivec2(1)) * gTex);
	float fDepth = texture(gSourceDepth, mTex).r;//texelFetch(gSourceDepth, iPosition, 0).r;
	vec2 mXYPos = mTex;

	vec3 mWPos = vec3(mXYPos, fDepth);
	mWPos = 2.0f * mWPos - 1.0f;

	vec4 mPos = gProjViewInv_Source * vec4(mWPos, 1.0f);
	mWPos.xyz = mPos.xyz * (1.0f / mPos.w);
	return mWPos.xyz;
}
////////////////////////////////////////////////////
vec3 ProjWorldPositionToSource( vec3 mWorldPos )
{
	vec4 mPPos = gProjView_Source * vec4(mWorldPos, 1.0f);
	mPPos.xyz *= (1.0f / mPPos.w);
	mPPos.xy = mPPos.xy * vec2(0.5f) + vec2(0.5f);
	return mPPos.xyz;
}
////////////////////////////////////////////////////
vec4 Reflection(vec3 mWorldPos, vec3 mWorldNorm, vec2 mTexCoord) 
{
	//////////////////////////////////////////////////////////////////////
	vec3 mEyeDir = normalize(mWorldPos - gCamPos);
	vec3 mReflectDir = normalize(reflect(mEyeDir, mWorldNorm));
	//////////////////////////////////////////////////////////////////////
	vec3 mCurrentRay = vec3(0.0f);
	vec3 mUVZ = vec3(0.0f);
	//////////////////////////////////////////////////////////////////////
	float mDelta = 128.0f;
	float L = mDelta;
	float mLDel = 0.00000001f;//0.004f;
	float mError;
	//////////////////////////////////////////////////////////////////////
	for (int i = 0; i < SSR_ITTERATION; ++i) 
	{
		mCurrentRay = mWorldPos + L * mReflectDir;
		mUVZ = ProjWorldPositionToSource(mCurrentRay);

		vec3 mNextPos = RestoteWorldPositionSource(mUVZ.xy);
		L = length(mWorldPos - mNextPos);
	}
	//////////////////////////////////////////////////////////////////////
	float mFresnel = clamp(0.4f + 2.8f * pow(1.0f + dot(mWorldNorm, mEyeDir), 2.0f), 0.0f, 1.0f); //max(dot(mWorldNorm, mEyeDir), 0.0f);
	mError = 1.0f - clamp(L * mLDel, 0.0f, 1.0f);
	float mCorrect = mFresnel * mError *
		smoothstep(-0.1, 0.0, mUVZ.x) * smoothstep(-0.1, 0.0, mUVZ.y) *
		smoothstep(mUVZ.x - 0.1, mUVZ.x, 1.0) * smoothstep(mUVZ.y - 0.1, mUVZ.y, 1.0);
	//////////////////////////////////////////////////////////////////////
	return texture(gSourceImage, mUVZ.xy) * mCorrect + texture(gSourceImage, mTexCoord) * (1.0f - mCorrect);
	//0.4f + 3.0f * (1.0f - mCorrect)
	//+ mReflectDir.xz * 0.4f, 5.0f * (1.0f - mCorrect)
	//////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////
void main() 
{
	vec3 mWorldPos = RestoteWorldPosition(gTex);
	vec3 mWorldNormal = normalize(texture(gNormal, gTex).xyz);
	////////////////////////////////////////////////////
	vec4 mReflection = Reflection(mWorldPos, mWorldNormal, gTex);
	vec4 mDiffuse = texture(gDiffuse, gTex);
	vec4 mSpecular = texture(gSpecular, gTex);
	////////////////////////////////////////////////////
	gResColor = mDiffuse + pow(mSpecular, vec4(0.3f)) * mReflection;
}

