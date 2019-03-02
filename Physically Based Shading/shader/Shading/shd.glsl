#version 440 core
//#pragma debug(on)
////////////////////////////////////////////////////
#define MAX_NUM_SHADOWCASTING_SPOTS 16
#define MAX_NUM_SHADOWCASTING_POINTS 16
////////////////////////////////////////////////////
#define POINT_LIGHT_SHADOW_CONST 0.0000f
////////////////////////////////////////////////////
#define TILE_RES 32
////////////////////////////////////////////////////
#define FLT_MAX         3.402823466e+38F
////////////////////////////////////////////////////
#define K_KOEF 0.79788456080286535587989211986876f// 0.5f
#define SQR_PI 1.7724538509055160272981674833411f// 0.5f
////////////////////////////////////////////////////
#define PI 3.1415926535897932384626433832795f
////////////////////////////////////////////////////
////////////////////////////////////////////////////
#define NUM_THREADS_X TILE_RES
#define NUM_THREADS_Y TILE_RES
#define NUM_THREADS_PER_TILE (NUM_THREADS_X*NUM_THREADS_Y)
////////////////////////////////////////////////////
layout(local_size_x = NUM_THREADS_X, local_size_y = NUM_THREADS_Y) in;
////////////////////////////////////////////////////
#define MAX_NUM_LIGHTS_PER_TILE 24
////////////////////////////////////////////////////
#define POINTLIGHT_SHADOW_POS 21 + MAX_NUM_SHADOWCASTING_SPOTS
////////////////////////////////////////////////////
//input gbuffer
layout(location = 0, binding = 0) uniform sampler2D g_depth;
layout(location = 1, binding = 1) uniform sampler2D g_normal;
layout(location = 2, binding = 2) uniform sampler2D g_diffuse;
layout(location = 3, binding = 3) uniform sampler2D g_specular;
layout(location = 64, binding = 11) uniform sampler2D g_PBR;//PBR Mettalic; Roughness; F0
////////////////////////////////////////////////////
//input light param
layout(location = 8, binding = 4) uniform samplerBuffer g_PointColor;
layout(location = 9, binding = 5) uniform samplerBuffer g_PointPositionRadius;
////////////////////////////////////////////////////
layout(location = 10, binding = 6) uniform samplerBuffer g_SpotColor;
layout(location = 11, binding = 7) uniform samplerBuffer g_SpotBoundingSpherePositionRadius;
layout(location = 12, binding = 8) uniform samplerBuffer g_SpotDirectionRadiusCosineAngle;
////////////////////////////////////////////////////
struct ParalelLight
{
	vec3 direction;
	vec4 diffuse;
	vec4 specular;
};
////////////////////////////////////////////////////
layout(std140) uniform day_night
{
	ParalelLight light_data[2];
};

layout(location = 6) uniform mat4 g_inv_proj_view;
layout(location = 7) uniform vec3 g_vCameraPos;
////////////////////////////////////////////////////
layout(location = 14) uniform mat4 g_mProjectionInv;
layout(location = 15) uniform mat4 g_mView;
layout(location = 16) uniform int g_uNumLights; 
layout(location = 17) uniform int g_sNumSpotLights;
layout(location = 18) uniform ivec2 g_sTexDim;
layout(location = 19) uniform mat4 g_mInvProjViewInvViewport;
////////////////////////////////////////////////////
layout(location = 20, binding = 9) uniform sampler2DArrayShadow gSpotLightShadowMap;//sampler2DArray
layout(location = 21) uniform mat4 g_mProjViewSpot[MAX_NUM_SHADOWCASTING_SPOTS];
////////////////////////////////////////////////////
layout(location = POINTLIGHT_SHADOW_POS, binding = 10) uniform samplerCubeArrayShadow gPointLightShadowMap;//sampler2DArray samplerCubeArrayShadow samplerCubeArray
//layout(location = POINTLIGHT_SHADOW_POS + 1) uniform mat4 g_mProjViewPoint[MAX_NUM_SHADOWCASTING_POINTS];
layout(location = POINTLIGHT_SHADOW_POS + 1) uniform vec4 g_mNF[MAX_NUM_SHADOWCASTING_POINTS];
////////////////////////////////////////////////////
layout(location = POINTLIGHT_SHADOW_POS + 18, binding = 12) uniform samplerCube gEnvMap;
////////////////////////////////////////////////////
//output data
layout(location = 4, binding = 0, rgba8) uniform image2D g_albedro;
layout(location = 5, binding = 1, rgba8) uniform image2D g_spec_albedro;
////////////////////////////////////////////////////
layout(location = 13) uniform ivec2 gTileCount;
////////////////////////////////////////////////////
//utilite function
////////////////////////////////////////////////////
/*if( bDoShadow )
        {
            float fShadowResult = DoSpotShadow( nLightIndex, vPosition );
            LightColorDiffuseResult  *= fShadowResult;
            LightColorSpecularResult *= fShadowResult;
        }*/
/*float DoSpotShadow( uint nShadowIndex, in float3 vPosition )
{
    float4 shadowTexCoord = mul( float4( vPosition, 1 ), g_mSpotShadowViewProj[ nShadowIndex ] );
    shadowTexCoord.xyz = shadowTexCoord.xyz / shadowTexCoord.w;

    shadowTexCoord.x = shadowTexCoord.x/2 + 0.5;
    shadowTexCoord.y = shadowTexCoord.y/-2 + 0.5;

    shadowTexCoord.x += nShadowIndex;
    shadowTexCoord.x *= rcp( MAX_NUM_SHADOWCASTING_SPOTS );

    shadowTexCoord.z -= g_ShadowBias.w;

    return FilterShadow( g_SpotShadowAtlas, shadowTexCoord.xyz );
}*/
/*float FilterShadow( Texture2D atlas, float3 uv )
{
    float shadow = 0.0;

    static const int kernelLevel = 2;
    static const int kernelWidth = 2*kernelLevel + 1;
    [unroll] for ( int i = -kernelLevel; i <= kernelLevel; i++ )
    {
        [unroll] for ( int j = -kernelLevel; j <= kernelLevel; j++ )
        {
            shadow += atlas.SampleCmpLevelZero( g_ShadowSampler, uv.xy, uv.z, int2( i, j ) ).r;
        }
    }

    shadow /= (kernelWidth*kernelWidth);

    return shadow;
}*/
/*
float PCF(in vec4 smCoord) 
{
	float res = 0.0f;

	res = textureProjOffset

	return res / 1.0f;
}*/

float VecToDepth(in vec3 mWorldPos, in float A, in float B, in float E)
{
	vec3 absVec = abs(mWorldPos);
	float LocalZ = -max(absVec.x, max(absVec.y, absVec.z));

	float NormZ = A * LocalZ + B;
	NormZ /= E * LocalZ;

	NormZ = NormZ * 0.5f + 0.5f;
	return NormZ;
}

float DoPointShadow(in int nShadowIndex, in vec3 vPosition)
{
	vec4 mPointPos = texelFetch(g_PointPositionRadius, nShadowIndex);
	vec3 vDir = vPosition - mPointPos.xyz;

	vec4 mC = g_mNF[nShadowIndex];

	float mZ = VecToDepth(vDir, mC.x, mC.y, mC.z);
	
	return texture(gPointLightShadowMap, vec4(vDir, float(nShadowIndex)), mZ - POINT_LIGHT_SHADOW_CONST);
}

float DoSpotShadow(in int nShadowIndex, in vec3 vPosition) 
{
	vec4 shadowTexCoord = g_mProjViewSpot[nShadowIndex] * vec4(vPosition, 1.0f);
	//shadowTexCoord.xyz = shadowTexCoord.xyz * vec3(0.5f) + vec3(0.5f);
	shadowTexCoord.xyz *= 1.0f / shadowTexCoord.w;
	vec2 smTex = shadowTexCoord.xy * vec2(0.5f) + vec2(0.5f);
	float fPosZ = shadowTexCoord.z * 0.5f + 0.5f;

	float fMapZ = texture(gSpotLightShadowMap, vec4(smTex, float(nShadowIndex), fPosZ - 0.0001f)).r;
		//texelFetch(gSpotLightShadowMap, ivec3((smTex * vec2(512.0f)), 5), 0).r;
	//sampler2DArrayShadow​();

	/*if (fPosZ > fPosZ + 0.000001f)return 1.0f;
	else return 0.0f;*/
	/*fMapZ += 0.0025f - fPosZ;
	fMapZ = fMapZ < 0.0f ? 0.0f : 1.0f;
	return fMapZ;*/

	//fMapZ += 0.00025f - fPosZ;
	//fMapZ = fMapZ < 0.0f ? 0.0f : 1.0f;
	return fMapZ;// pow(fMapZ, 200.0f);
}
////////////////////////////////////////////////////
//PBS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fresnel Schlick function
vec3 fresnelSchlick(float fcosTheta, vec3 F0) 
{
	return F0 + (vec3(1.0f) - F0) * pow(1.0f - fcosTheta, 5.0f);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Geometry Smith Function

float GeometrySchilckBeckmannGGX(float fNdotV, float fRoughness)//fNdotV - cos
{
	float a = (fRoughness + 1.0f) * (fRoughness + 1.0f);//fRoughness * fRoughness;//(fRoughness + 1.0f);
	float k = a / 8.0f;//* 0.5f;//K_KOEF;//(R * R) / 8.0f;//(fRoughness * fRoughness) * sqrt(2/PI);
	//float k = a * a * SQR_PI;//
	float nom = fNdotV;
	float denom = fNdotV * (1.0f - k) + k;
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float fRoughness) 
{
	float fNdotV = max(dot(N, V), 0.0f);
	float fNdotL = max(dot(N, L), 0.0f);

	float ggx1 = GeometrySchilckBeckmannGGX(fNdotV, fRoughness);
	float ggx2 = GeometrySchilckBeckmannGGX(fNdotL, fRoughness);

	return ggx1 * ggx2;
}

float G_Smith(float fRoughness, float fNdotV, float fNdotL) //vec3 N, vec3 V, vec3 L, float fRoughness)
{
	//float fNdotV = max(dot(N, V), 0.0f);
	//float fNdotL = max(dot(N, L), 0.0f);

	float ggx1 = GeometrySchilckBeckmannGGX(fNdotV, fRoughness);
	float ggx2 = GeometrySchilckBeckmannGGX(fNdotL, fRoughness);

	return ggx1 * ggx2;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Distribution GGX Function
float DistributionGGX(vec3 N, vec3 H, float fRoughness)
{
	float a = fRoughness * fRoughness;
	float a2 = a * a;
	float fNdotH = max(dot(N, H), 0.0f);
	float fNdotH2 = fNdotH * fNdotH;

	float nom = a2;
	float denom = (fNdotH2 * (a2 - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return nom / denom;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec3 Specular(in vec3 N, in vec3 H, in vec3 V, in vec3 L, in float fRoughness, in vec3 F0)
{
	float NDF = DistributionGGX(N, H, fRoughness);
	float G = GeometrySmith(N, V, L, fRoughness);
	vec3 F = fresnelSchlick(max(dot(V, H), 0.0f), F0);//(H, V)

	vec3 nominator = F * G * NDF;
	float denominator = 4.0f * max(dot(V, N), 0.0f) * max(dot(L, N), 0.0f) + 0.0001f;
	vec3 brdf = nominator / denominator;
	return brdf;
}
////////////////////////////////////////////////////
vec3 BlendDielectric(vec3 kDiff, vec3 Kspec, vec3 kBase)
{
	return kDiff + Kspec;
}
////////////////////////////////////////////////////
vec3 BlendMetal(vec3 kDiff, vec3 Kspec, vec3 kBase) 
{
	return Kspec * kBase;// + kDiff*0.01f;
}
////////////////////////////////////////////////////
vec3 BlendMaterial(vec3 kDiff, vec3 Kspec, vec3 kBase, float Kmetallic)
{
	float scRange = smoothstep(0.25, 0.85, Kmetallic);
	vec3 dielectric = BlendDielectric(kDiff, Kspec, kBase);
	vec3 metallic = BlendMetal(kDiff, Kspec, kBase);
	return mix(dielectric, metallic, scRange);
}
////////////////////////////////////////////////////
vec4 BlendMaterial(vec4 kDiff, vec4 Kspec, float Kmetallic)
{
	float scRange = smoothstep(0.25, 0.45, Kmetallic);
	return mix(kDiff, Kspec, scRange);
}
////////////////////////////////////////////////////
/*vec3 Brdf(in vec3 N, in vec3 H, in vec3 V, in vec3 L, in float fRoughness, in vec3 Kdiff, in float Kmetallic, in vec3 F0)
{
	vec3 Kspec = Specular(N, H, V, L, fRoughness, F0);
	return BlendMaterial(Kdiff, Kspec, Kdiff * PI, Kmetallic);
}*/
////////////////////////////////////////////////////
//vec3 BRDFLighting(in vec3 N, in vec3 H, in vec3 V, in vec3 L, in float fRoughness, in vec3 Kdiff, in float Kmetallic, in vec3 F0)
//{
//	Brdf(N, H, V, L, fRoughness, Kdiff, Kmetallic, F0);
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec3 BRDF(in vec3 N, in vec3 H, in vec3 V, in vec3 L, in float fRoughness, in vec3 F0)
{
	float NDF = DistributionGGX(N, H, fRoughness);
	float G = GeometrySmith(N, V, L, fRoughness);
	vec3 F = fresnelSchlick(max(dot(V, H), 0.0f), F0);//(H, V)

	vec3 nominator = NDF * G * F;
	float denominator = 4.0f * max(dot(V, N), 0.0f) * max(dot(L, N), 0.0f) + 0.0000001f;
	vec3 brdf = nominator / denominator;
	return brdf;
}
////////////////////////////////////////////////////
float ExtractDepth(ivec2 id) 
{
	float depth = texelFetch(g_depth, id, 0).r;
	depth = depth * 2.0f - 1.0f;

	return depth;
}
////////////////////////////////////////////////////
vec3 ExtractNormal(vec4 tex_norm)
{
	vec3 normal = tex_norm.xyz;//texelFetch(g_normal, id, 0).xyz;
	//normal = normal*vec3(2.0f) - vec3(1.0f);//[0;1] -> [-1;1]
	normal = normalize(normal);
	//normal = normal*vec3(0.5f) + vec3(0.5f);//[-1;1]->[0;1]

	return normal;
}
////////////////////////////////////////////////////
vec3 ExtractPosition(ivec2 id, ivec2 texture_size)
{
	float depth = texelFetch(g_depth, id, 0).r;
	depth = depth * 2.0f - 1.0f;

	//vec2 msize = vec2(texture_size.x - 1.0f, texture_size.y - 1.0f);
	vec2 msize = vec2(texture_size);

	vec2 uv = vec2(id) / msize;
	uv = uv*vec2(2.0f) - vec2(1.0f);

	vec4 pos = vec4(uv, depth, 1.0f);
	pos = g_inv_proj_view * pos;
	pos.xyz *= (1.0f / pos.w);

	return pos.xyz;
}
////////////////////////////////////////////////////
vec4 ConvertProjToView(vec4 p)
{
	p = g_mProjectionInv * p;
	p.xyz /= p.w;
	return p;//view position
}
////////////////////////////////////////////////////
float ConvertProjDepthToView( float z ) 
{
	//z = z * 2.0f - 1.0;

	z = g_mProjectionInv[3][2] / ( g_mProjectionInv[2][3] * z + g_mProjectionInv[3][3] );
	//z = 1.f / (z*g_mProjectionInv._34 + g_mProjectionInv._44);
	//z = (0.5 * g_mProjectionInv[3][2]) / (z + 0.5 * g_mProjectionInv[2][2] - 0.5);

	return z;
}
////////////////////////////////////////////////////
float UnpackZ( float z ) 
{
	return z * 2.0f - 1.0f;
}
////////////////////////////////////////////////////
vec4 CreatePlaneEquation(vec4 b, vec4 c)
{
	vec4 n;
	n.xyz = normalize(cross(b.xyz, c.xyz));
	n.w = 0.0f;
	return n;
}
////////////////////////////////////////////////////
float GetSignedDistanceFromPlane( vec4 p, vec4 eqn ) 
{
	return dot(p.xyz, eqn.xyz);
}
////////////////////////////////////////////////////
float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
vec2 Hammersley(uint i, uint N) {
	return vec2(float(i) / float(N), radicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N)
{
	float a = Roughness * Roughness;
	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
	float SinTheta = sqrt(1 - CosTheta * CosTheta);
	vec3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;
	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 TangentX = normalize(cross(UpVector, N));
	vec3 TangentY = cross(N, TangentX);
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

/*vec3 importanceSampleGGX(vec2 sample, float m, vec3 N) {
	float phi = sample.x * 2.0f * PI;
	float cosTheta = sqrt((1.0f - sample.y) / 1.0f + (m*m - 1.0f) * sample.y);
	float sinTheta = sqrt(1.0f - cosTheta*cosTheta);

	vec3 vector = vec3(
		sinTheta * cos(phi),
		sinTheta * sin(phi),
		cosTheta
	);


	return vector;
	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangentX = normalize(cross(up, N));
	vec3 tangentY = normalize(cross(N, tangentX));
	// Project 
	return tangentX * vector.x + tangentY * vector.y + N * vector.z;
}*/
#define NUMBER_OF_SAMPLES 128


/*vec2 IntegrateBRDF(float Roughness, float NoV) {
	vec3 V;
	V.x = sqrt(1.0f - NoV * NoV); // sin
	V.y = 0.0;
	V.z = NoV;
	// cos
	float A = 0.0;
	float B = 0.0;

	for (uint i = 0u; i < NUMBER_OF_SAMPLES; i++)
	{
		vec2 Xi = Hammersley(i, NUMBER_OF_SAMPLES);
		vec3 H = ImportanceSampleGGX(Xi, Roughness, vec3(0.0, 0.0, 1.0));
		vec3 L = 2.0 * dot(V, H) * H - V;
		float NoL = max(L.z, 0.0);
		float NoH = max(H.z, 0.0);
		float VoH = max(dot(V, H), 0.0);

		if (NoL > 0)
		{
			float G = G_Smith(Roughness, NoV, NoL);
			float G_Vis = G * VoH / (NoH * NoV);
			float Fc = pow(1.0 - VoH, 5.0);
			A += (1.0 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	return vec2((A) / float(NUMBER_OF_SAMPLES), (B) / float(NUMBER_OF_SAMPLES));
}*/
//F0 + (vec3(1.0f) - F0) * pow(1.0f - fcosTheta, 5.0f);
//max(dot(V, H), 0.0f), F0
vec3 SpecularIBL(vec3 SpecularColor, float Roughness, vec3 N, vec3 V)
{
	vec3 SpecularLighting = vec3(0.0f);
	const uint NumSamples = 1024;
	for (uint i = 0; i < NumSamples; i++)
	{
		vec2 Xi = Hammersley(i, NumSamples);
		vec3 H = ImportanceSampleGGX(Xi, Roughness, N);
		vec3 L = 2 * dot(V, H) * H - V;
		float NoV = max(dot(N, V), 0.0f);
		float NoL = max(dot(N, L), 0.0f);
		float NoH = max(dot(N, H), 0.0f);
		float VoH = max(dot(V, H), 0.0f);
		if (NoL > 0)
		{
			vec3 SampleColor = texture(gEnvMap, L).rgb;//EnvMap.SampleLevel(EnvMapSampler, L, 0).rgb;
			float G = G_Smith(Roughness, NoV, NoL);
			float Fc = pow(1 - VoH, 5);
			vec3 F = //SpecularColor + (vec3(1.0f) - SpecularColor) * pow(1.0f - VoH, 5.0f);
			(1 - Fc) * SpecularColor + Fc;
			// Incident light = SampleColor * NoL
			// Microfacet specular = D*G*F / (4*NoL*NoV)
			// pdf = D * NoH / (4 * VoH)
			SpecularLighting += SampleColor * F * G * VoH / (NoH * NoV);
		}
	}
	return SpecularLighting / NumSamples;
}
////////////////////////////////////////////////////
#define MAIN_VAR
////////////////////////////////////////////////////
void compute_paralel_light(ParalelLight data_light, vec3 normal, vec3 position, float spec_pow, vec4 diff, vec4 spec, out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient = vec4(0.0f);
	diffuse = vec4(0.0f);
	specular = vec4(0.0f);

	vec3 li_dir = normalize(data_light.direction);

	ambient = diff * diff.a * data_light.diffuse * data_light.diffuse.a;
	ambient.a = 1.0f;

	diffuse = diff * data_light.diffuse * max(0.0f, dot(normal, -li_dir)) * 0.34f;
	diffuse.a = 1.0f;

	vec3 r = normalize(reflect(li_dir, normal));
	vec3 v = normalize(g_vCameraPos - position);

	specular = spec * data_light.specular * spec.a * pow(max(0.0f, dot(v, r)), spec_pow) * 0.6f;
	specular.a = spec.a;
}
////////////////////////////////////////////////////
void DoPointLighting(in int nLightIndex, in vec3 normal, in vec3 position, in vec3 vViewDir, in float spec_pow, in float fRoughness, in vec3 Kdiff, in float fMetallic, in vec3 F0, in vec4 mEnvColor, in vec4 mIrRadiance,
	out vec4 LightColorDiffuseResult, out vec4 LightColorSpecularResult)
{
	//view_dir - its direction of camera pos to camera view(so its z axis in view space)
	vec4 center_radius = texelFetch(g_PointPositionRadius, nLightIndex);
	vec4 color = texelFetch(g_PointColor, nLightIndex);

	vec3 toLight = center_radius.xyz - position;
	vec3 toLightDir = normalize(toLight);
	float gLightDistance = length(toLight);

	LightColorDiffuseResult = vec4(0.0f);
	LightColorSpecularResult = vec4(0.0f);

	float fRad = center_radius.w;
	if (gLightDistance < fRad) 
	{
		float x = gLightDistance / fRad;
		// fake inverse squared falloff:
		// -(1/k)*(1-(k+1)/(1+k*x^2))
		// k=20: -(1/20)*(1 - 21/(1+20*x^2))
		float fFalloff = -0.05f + (1.05f / (1.0f + (20.0f * x * x)));
		//fFalloff = 1.0f;//----
		//++++++LightColorDiffuseResult = color * fFalloff * clamp( dot(normal, toLightDir), 0.0f, 1.0f) * color.a;
		//LightColorDiffuseResult.a = color.a;

		vec3 vHalfAngle = normalize(vViewDir + toLightDir);//blinn lighting
		//++++++LightColorSpecularResult = color * fFalloff * pow( clamp( dot(vHalfAngle, normal), 0.0f, 1.0f), spec_pow ) * pow(color.a, 1.8f);//spec_pow or 8

		float smShadow = DoPointShadow(nLightIndex, position);

		//BRDF(in vec3 N, in vec3 H, in vec3 V, in vec3 L, in float fRoughness, in vec3 F0)

		color *= pow(color.a, 0.65f);

		float alpha = 0.1f;
		float betta = 1.0f;

		vec3 N = normal;
		vec3 V = normalize(g_vCameraPos - position);
		vec3 L = toLightDir; //vec3(sin(alpha) * cos(betta), cos(alpha), sin(alpha) * sin(betta));//toLightDir;
		vec3 H = normalize(V + L);
		vec4 mColorD;

#ifdef MAIN_VAR
		mColorD = color * fFalloff * max(dot(N, L), 0.0f);
		//vec3 brdf = BRDF(N, H, V, L, fRoughness, F0);
		//fMetallic = 1.0f;
		vec3 kS = vec3(0.0f);//pow(fMetallic, 8.0f));//0.0f);//fresnelSchlick(max(dot(H, V), 0.0f), vec3(1.00f, 0.86f, 0.57f));// F0);//vec3(0.0f);//fresnelSchlick(max(dot(H, V), 0.0f), F0);
		vec3 kD = vec3(1.0f) - kS;
		kD *= 1.0f - fMetallic;
		//kS = vec3(1.0f) - kD;

		LightColorDiffuseResult = vec4(kD, 1.0f) * mColorD;// *texture(gEnvMap, N, 12.0f) * PI;// *mIrRadiance;
		LightColorSpecularResult = vec4(SpecularIBL(vec3(1.00, 0.86, 0.57), fRoughness, N, V), 1.0f) * mColorD;//color * fFalloff;//vec4(brdf, 1.0f) * mColorD * PI * texture(gEnvMap, reflect(-V, N), 2.1f * fRoughness);
		//LightColorSpecularResult += LightColorSpecularResult * texture(gEnvMap, reflect(-V, N));//;
															  // *pow(mEnvColor, vec4(0.8f));//vec4(brdf, 1.0f) * mColorD * mEnvColor;
		//vec3(1.00, 0.71, 0.29) - gold
		//vec3(0.24, 0.24, 0.24) - plastic
		//vec3(0.77, 0.78, 0.78) - iron
		//silver - (0.95, 0.93, 0.88)
		//LightColorDiffuseResult *= smShadow;
		//LightColorSpecularResult *= smShadow;
#else
		mColorD = color * fFalloff * max(dot(N, L), 0.0f);
		vec3 kDiff = Kdiff;// *mIrRadiance.rgb; //normalize(reflect(-toLight, N)
		vec3 Kspec = SpecularIBL(vec3(1.00f, 0.86f, 0.57f), fRoughness, N, V);
			//Specular(N, H, V, L, fRoughness, F0) * textureLod(gEnvMap, reflect(-V, N), 4.0 * (fRoughness)).rgb * PI;// *mEnvColor.rgb;  reflect(-V, N)

		//kDiff *= smShadow;
		//Kspec *= smShadow;

		vec3 dielectric = BlendDielectric(Kdiff, Kspec, Kdiff * PI);
		vec3 metal = BlendMetal(Kdiff, Kspec, Kdiff * PI);

		LightColorDiffuseResult = vec4(dielectric, 1.0f) * fFalloff * max(dot(N, L), 0.0f);// *mColorD;
		LightColorSpecularResult = vec4(metal, 1.0f) * PI;// *mColorD;
#endif
	}
}
////////////////////////////////////////////////////
void DoSpotLighting(in int nLightIndex, in vec3 vNorm, in vec3 vPosition, in vec3 vViewDir, in float spec_pow,
	out vec4 LightColorDiffuseResult, out vec4 LightColorSpecularResult)
{
	vec4 BoundingSphereCenterAndRadius = texelFetch(g_SpotBoundingSpherePositionRadius, nLightIndex);
	vec4 SpotParam = texelFetch(g_SpotDirectionRadiusCosineAngle, nLightIndex);

	vec3 SpotLightDir;
	SpotLightDir.xy = SpotParam.xy;
	SpotLightDir.z = sqrt(1.0f - SpotParam.x*SpotParam.x - SpotParam.y*SpotParam.y);

	SpotLightDir.z = (SpotParam.z > 0.0) ? SpotLightDir.z : -SpotLightDir.z;

	//SpotLightDir = normalize(SpotLightDir);

	vec3 LightPosition = BoundingSphereCenterAndRadius.xyz - SpotLightDir * BoundingSphereCenterAndRadius.w;
	vec3 vToLight = LightPosition - vPosition;
	vec3 vToLightNormalized = normalize(vToLight);
	float fLightDistance = length(vToLight);
	float fCosineOfCurrentConeAngle = dot(-vToLightNormalized, SpotLightDir);

	LightColorDiffuseResult = vec4(0.0f);
	LightColorSpecularResult = vec4(0.0f);

	float fRad = SpotParam.w;
	float fCosineOfConeAngle = (SpotParam.z > 0.0) ? SpotParam.z : -SpotParam.z;

	vec4 gSpotColor = texelFetch(g_SpotColor, nLightIndex);
	if (fLightDistance < fRad && fCosineOfCurrentConeAngle > fCosineOfConeAngle) 
	{
		float fRadialAttenuation = (fCosineOfCurrentConeAngle - fCosineOfConeAngle) / (1.0 - fCosineOfConeAngle);
		fRadialAttenuation = fRadialAttenuation * fRadialAttenuation;

		float x = fLightDistance / fRad;
		// fake inverse squared falloff:
		// -(1/k)*(1-(k+1)/(1+k*x^2))
		// k=20: -(1/20)*(1 - 21/(1+20*x^2))
		float vPowValue = gSpotColor.a * 255.0f;
		float fFalloff = -0.05 + 1.05 / (1 + 20 * x*x);
		LightColorDiffuseResult = gSpotColor * fFalloff * fRadialAttenuation * max(dot(vNorm, vToLightNormalized), 0.0f) * vPowValue;

		vec3 vHalfAngle = normalize(vViewDir + vToLightNormalized);//blinn lighting
		LightColorSpecularResult = gSpotColor * fFalloff * fRadialAttenuation * pow(max(dot(vHalfAngle, vNorm), 0.0f), spec_pow) * pow(vPowValue, 1.5f);//spec_pow or 8

		float smShadow = DoSpotShadow(nLightIndex, vPosition);

		LightColorDiffuseResult *= smShadow;
		LightColorSpecularResult *= smShadow;
	}
}
////////////////////////////////////////////////////
//groupshared variable
////////////////////////////////////////////////////
shared uint ldsZMax;
shared uint ldsZMin;

shared uint ldsLightIdxCounterA;
shared uint ldsLightIdxCounterB;
shared uint ldsLightIdx[2 * MAX_NUM_LIGHTS_PER_TILE];

shared uint ldsSpotIdxCounterA;
shared uint ldsSpotIdxCounterB;
shared uint ldsSpotIdx[2 * MAX_NUM_LIGHTS_PER_TILE];
////////////////////////////////////////////////////
void CalculateMinMaxDepthInLds(ivec3 globalIdx)
{
	float pixelDepth = ExtractDepth(globalIdx.xy);// = texelFetch(g_depth, globalIdx.xy, 0).x;
	//pixelDepth = pixelDepth * 2.0f - 1.0;
	//pixelDepth = -pixelDepth;
	//pixelDepth = UnpackZ(pixelDepth);

	float pixelViewPosZ = (ConvertProjDepthToView(pixelDepth));
	uint pixelZ = floatBitsToUint(pixelViewPosZ);// pixelViewPosZ);//floatBitsToUint(pixelViewPosZ); uint
	//uint opaqueZ = asuint(opaqueViewPosZ);

	if (pixelDepth != 1.0f)
	{
		atomicMin(ldsZMin, pixelZ);//atomicMax
		atomicMax(ldsZMax, pixelZ);//atomicMin
	}
}
////////////////////////////////////////////////////
void DoLightingCulling(in ivec3 globalIdx, in int localIdxFlattened, in ivec3 groupIdx, out float fHalfZ)
{
	if ( localIdxFlattened == 0 ) 
	{
		ldsZMin = 0xffffffff;// 0x3f800000;//0x7f7fffff;// uint(100000000.0f);//0xffffffff;//0x7f7fffff;// 0xffffffff;//uint(0xFFFFFFFF);//0x7f7fffff;
		ldsZMax = 0x00000000;// 0x000000;//uint(0.0f);//0x0;//0x800000;// 0x7fffe1;// 0xff7fffff;
		ldsLightIdxCounterA = 0;
		ldsLightIdxCounterB = MAX_NUM_LIGHTS_PER_TILE;
		ldsSpotIdxCounterA = 0;
		ldsSpotIdxCounterB = MAX_NUM_LIGHTS_PER_TILE;
	}

	vec4 frustumEqn[4];
	{
		int pxm = TILE_RES * groupIdx.x;
		int pym = TILE_RES * groupIdx.y;
		int pxp = TILE_RES * (groupIdx.x + 1);
		int pyp = TILE_RES * (groupIdx.y + 1);

		uint uWindowWidthEvenlyDivisibleByTileRes = TILE_RES * gTileCount.x;
		uint uWindowHeightEvenlyDivisibleByTileRes = TILE_RES * gTileCount.y;

		float fWindowWidthEvenlyDivisibleByTileRes = 1.0f / float(uWindowWidthEvenlyDivisibleByTileRes);
		float fWindowHeightEvenlyDivisibleByTileRes = 1.0f / float(uWindowHeightEvenlyDivisibleByTileRes);

		vec4 frustum[4];

		frustum[0] = ConvertProjToView( vec4( (pxm * fWindowWidthEvenlyDivisibleByTileRes * 2.0f) - 1.0f, (pym * fWindowHeightEvenlyDivisibleByTileRes * 2.0f) - 1.0f, 1.0f, 1.0f) );
		frustum[1] = ConvertProjToView( vec4( (pxp * fWindowWidthEvenlyDivisibleByTileRes * 2.0f) - 1.0f, (pym * fWindowHeightEvenlyDivisibleByTileRes * 2.0f) - 1.0f, 1.0f, 1.0f) );
		frustum[2] = ConvertProjToView( vec4( (pxp * fWindowWidthEvenlyDivisibleByTileRes * 2.0f) - 1.0f, (pyp * fWindowHeightEvenlyDivisibleByTileRes * 2.0f) - 1.0f, 1.0f, 1.0f) );
		frustum[3] = ConvertProjToView( vec4( (pxm * fWindowWidthEvenlyDivisibleByTileRes * 2.0f) - 1.0f, (pyp * fWindowHeightEvenlyDivisibleByTileRes * 2.0f) - 1.0f, 1.0f, 1.0f) );

		for (int i = 0; i < 4; i++)
			frustumEqn[i] = CreatePlaneEquation(frustum[i], frustum[(i+1)&3]);
	}

	barrier();

	CalculateMinMaxDepthInLds(globalIdx);

	barrier();
	float maxZ = uintBitsToFloat(ldsZMax);//uintBitsToFloat(ldsZMax); float
	float minZ = uintBitsToFloat(ldsZMin);//uintBitsToFloat(ldsZMin);

	//maxZ = ConvertProjDepthToView(UnpackZ(maxZ));
	//minZ = ConvertProjDepthToView(UnpackZ(minZ));

	/*if (maxZ < minZ) 
	{
		float t = maxZ;
		maxZ = minZ;
		minZ = t;
	}*/


	fHalfZ = (maxZ + minZ) * 0.5f;

	for (int i = localIdxFlattened; i < g_uNumLights; i += NUM_THREADS_PER_TILE) 
	{
		vec4 center = texelFetch(g_PointPositionRadius, i);
		float r = center.w;
		center.xyz = (g_mView * vec4(center.xyz, 1.0f)).xyz;

		if (GetSignedDistanceFromPlane(center, frustumEqn[0]) < r &&
			GetSignedDistanceFromPlane(center, frustumEqn[1]) < r &&
			GetSignedDistanceFromPlane(center, frustumEqn[2]) < r &&
			GetSignedDistanceFromPlane(center, frustumEqn[3]) < r)
		{
			if (center.z - minZ < r && center.z + r > fHalfZ)
			{
				uint dstIdx = 0;
				dstIdx = atomicAdd(ldsLightIdxCounterA, 1);
				ldsLightIdx[dstIdx] = i;
			}

			if (center.z - fHalfZ < r && center.z + r > maxZ)
			{
				uint dstIdx = 0;
				dstIdx = atomicAdd(ldsLightIdxCounterB, 1);
				ldsLightIdx[dstIdx] = i;
			}
		}
	}

	for (int j = localIdxFlattened; j < g_sNumSpotLights; j += NUM_THREADS_PER_TILE)
	{
		vec4 center = texelFetch(g_SpotBoundingSpherePositionRadius, j);
		float r = center.w;
		center.xyz = (g_mView * vec4(center.xyz, 1.0f)).xyz;

		if (GetSignedDistanceFromPlane(center, frustumEqn[0]) < r &&
			GetSignedDistanceFromPlane(center, frustumEqn[1]) < r &&
			GetSignedDistanceFromPlane(center, frustumEqn[2]) < r &&
			GetSignedDistanceFromPlane(center, frustumEqn[3]) < r)
		{
			if (center.z - minZ < r && center.z + r > fHalfZ)
			{
				uint dstIdx = 0;
				dstIdx = atomicAdd(ldsSpotIdxCounterA, 1);
				ldsSpotIdx[dstIdx] = j;
			}

			if (center.z - fHalfZ < r && center.z + r > maxZ)
			{
				uint dstIdx = 0;
				dstIdx = atomicAdd(ldsSpotIdxCounterB, 1);
				ldsSpotIdx[dstIdx] = j;
			}
		}
	}


	barrier();
}
////////////////////////////////////////////////////
void main()
{
	ivec3 globalIdx = ivec3(gl_GlobalInvocationID);
	int localIdxFlattened = int(gl_LocalInvocationIndex);
	ivec3 groupIdx = ivec3(gl_WorkGroupID);

	float fHalfZ;
	DoLightingCulling(globalIdx, localIdxFlattened, groupIdx, fHalfZ);


	//ivec2 texture_size = imageSize(g_albedro);
	//ivec2 cs_id = ivec2(gl_GlobalInvocationID.xy);
	//if (gl_GlobalInvocationID.x < texture_size.x && gl_GlobalInvocationID.y <= texture_size.y) 
	if(globalIdx.x < g_sTexDim.x && globalIdx.y < g_sTexDim.y)
	{
		//float depth = texelFetch(g_depth, cs_id, 0).r;
		//if (depth < 0.0)depth = 0.0f;
		//depth = depth*0.5f + 0.5f;
		//depth = pow(depth, 128.0);
		
		//imageStore(g_albedro, cs_id, vec4(depth));
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//Diffuse
		vec4 DiffuseTex = vec4(1.0f);//0.77f, 0.78f, 0.78f, 1.0f);//1.0f);//texelFetch(g_diffuse, globalIdx.xy, 0);
		vec4 SpecularTex = texelFetch(g_specular, globalIdx.xy, 0);
		float fSpecMask = SpecularTex.a;//reflective intensity
		//////////////////////////////////////////////////////////////
		vec4 gTexNorm = texelFetch(g_normal, globalIdx.xy, 0);

		vec4 mPBR = texelFetch(g_PBR, globalIdx.xy, 0);
		float fMetallic = mPBR.x;
		float fRoughness = mPBR.y;
#ifndef MAIN_VAR
		fRoughness = clamp(0.7f * fRoughness + 0.3f, 0.3f, 1.0f);
#else 
		//fRoughness = clamp(0.9f * fRoughness + 0.1f, 0.1f, 1.0f);
#endif
		vec3 F0 = vec3(0.04f, 0.04f, 0.04f);//1.00f, 0.86f, 0.57f);//mPBR.z);
		F0 = mix(F0, DiffuseTex.xyz, fMetallic);

		DiffuseTex = vec4(1.00, 0.71, 0.29, 1.0);//0.813, 0.686, 0.215, 1.0);// 0.05, 0.05, 0.05, 1.0);
			//0.05, 0.05, 0.05, 1.0);//1.00f, 0.71f, 0.29f, 1.0f);//vec4(0.04f);//vec4(0.57f, 0.43f, 0.36f, 1.0f);
		//vec4(1.00, 0.86, 0.57) - gold
		//vec4(0.56, 0.57, 0.58, 1.0) - iron
		//silver 0.98, 0.97, 0.95
		vec3 vNorm = ExtractNormal(gTexNorm);
		float fDepthBufferDepth = ExtractDepth(globalIdx.xy);

		vec4 vWorldSpacePosition = g_mInvProjViewInvViewport * vec4(float(globalIdx.x) + 0.5f, float(globalIdx.y) + 0.5f, fDepthBufferDepth, 1.0f);
		vec3 vPositionWS = vWorldSpacePosition.xyz * (1.0f / vWorldSpacePosition.w);
		vec3 vViewDir = normalize(g_vCameraPos - vPositionWS);

		vec3 reflactinDir = normalize(reflect(-vViewDir, vNorm));
		//reflactinDir.y = -reflactinDir.y;
		vec4 mEnvColor = textureLod(gEnvMap, reflactinDir, 4.0 * (fRoughness));
		vec4 mIrRadiance = textureLod(gEnvMap, vNorm, mix(4.8f, 9.0f, fRoughness));
		// vec4(1.0f);//
		vec4 AccumDiffuse = vec4(0.0f);
		vec4 AccumSpecular = vec4(0.0f);
		vec4 AccumAmbient = vec4(0.0f);
		float fViewPosZ = ConvertProjDepthToView(fDepthBufferDepth);

		//vec4 tmp;

		//loop over point light
		{
			uint uStartIdx = (fViewPosZ > fHalfZ) ? 0 : MAX_NUM_LIGHTS_PER_TILE;
			uint uEndIdx = (fViewPosZ > fHalfZ) ? ldsLightIdxCounterA : ldsLightIdxCounterB;
			
			for (uint i = uStartIdx; i < uEndIdx; i++)
			{
				uint nLightIndex = ldsLightIdx[i];

				//if (nLightIndex != 3)continue;

				vec4 LightColorDiffuseResult;
				vec4 LightColorSpecularResult;

				DoPointLighting(int(nLightIndex), vNorm, vPositionWS, vViewDir, gTexNorm.a, fRoughness, DiffuseTex.rgb, fMetallic, F0, mEnvColor, mIrRadiance, LightColorDiffuseResult, LightColorSpecularResult);

				AccumDiffuse += LightColorDiffuseResult;
				AccumSpecular += LightColorSpecularResult;
				//AccumAmbient += DiffuseTex * vec4(0.003f);
			}

			//tmp = (uEndIdx - uStartIdx <= 8) ? vec4(0.0f) : vec4(1.0f);
				//vec4((uEndIdx - uStartIdx) / float(MAX_NUM_LIGHTS_PER_TILE));
				//vec4(1.0f);
		}

		//loop over spot light
		{
			uint uStartIdx = (fViewPosZ > fHalfZ) ? 0 : MAX_NUM_LIGHTS_PER_TILE;
			uint uEndIdx = (fViewPosZ > fHalfZ) ? ldsSpotIdxCounterA : ldsSpotIdxCounterB;
			
			for (uint i = uStartIdx; i < uEndIdx; i++)
			{
				uint nLightIndex = ldsSpotIdx[i];

				vec4 LightColorDiffuseResult;
				vec4 LightColorSpecularResult;

				//DoSpotLighting(int(nLightIndex), vNorm, vPositionWS, vViewDir, gTexNorm.a, LightColorDiffuseResult, LightColorSpecularResult);

				//AccumDiffuse += LightColorDiffuseResult;
				//AccumSpecular += LightColorSpecularResult;
			}
		}


		/*// pump up the lights
    AccumDiffuse *= 2;
    AccumSpecular *= 8;
	*/
		AccumAmbient += DiffuseTex * vec4(0.03f);
		vec4 g_AmbientColorUp = vec4(0.013f, 0.015f, 0.050f, 1.0f);
		vec4 g_AmbientColorDown = vec4(0.0013f, 0.0015f, 0.0050f, 1.0f);
		float fAmbientBlend = 0.5f * vNorm.y + 0.5;
		vec4 Ambient = g_AmbientColorUp * fAmbientBlend + g_AmbientColorDown * (1 - fAmbientBlend) + AccumAmbient;
		vec4 DiffuseAndAmbient = AccumDiffuse + Ambient;
		
		vec4 ResultColor;
#ifdef MAIN_VAR
		vec4 ResultDiffuseAndAmbient = DiffuseTex * DiffuseAndAmbient;
		vec4 ResultSpecular = AccumSpecular *fSpecMask * SpecularTex;// DiffuseTex *fSpecMask * SpecularTex;//
		ResultColor = ResultDiffuseAndAmbient + ResultSpecular;//;
#else
		vec4 ResultDiffuse = AccumDiffuse;
		vec4 ResultSpecular = AccumSpecular;// *fSpecMask * SpecularTex * DiffuseTex;
		ResultColor =  BlendMaterial(ResultDiffuse, ResultSpecular, fMetallic) + Ambient;//
#endif
		//vec4 Result = DiffuseTex * (DiffuseAndAmbient + AccumSpecular * fSpecMask * SpecularTex);
		//**	vec4 ResultDiffuseAndAmbient = DiffuseTex * DiffuseAndAmbient;
	//**	vec4 ResultSpecular = DiffuseTex * AccumSpecular * fSpecMask * SpecularTex;//DiffuseTex * AccumSpecular * fSpecMask * SpecularTex;

		if (length(vPositionWS) > 600.0f && length(vPositionWS) < 2200.0f ) ResultColor = texture(gEnvMap, -reflactinDir);// vec4(1.0f);

		//ResultDiffuseAndAmbient = tmp + ResultDiffuseAndAmbient * 0.1f;
		// vec4(pow(texelFetch(gSpotLightShadowMap, ivec3(globalIdx.xy, 5), 0).r
		imageStore(g_spec_albedro, globalIdx.xy, ResultSpecular);
		imageStore(g_albedro, globalIdx.xy, vec4(ResultColor));// ResultColor);//DiffuseTex);// gTexNorm*vec4(0.5f) + vec4(0.5f));//ResultDiffuseAndAmbient);//
		//vec4(vec3(vNorm.xyz) * vec3(0.5f) + vec3(0.5f), 1.0f));//ResultDiffuseAndAmbient+ ResultSpecular);
		//////////////////////////////////////////////////////////////
		/*vec4 ambient;
		vec4 diffuse;
		vec4 specular;

		vec4 tex_norm = texelFetch(g_normal, cs_id, 0);

		vec3 normal = ExtractNormal(tex_norm);
		vec3 position = ExtractPosition(cs_id, texture_size);
		vec4 diff = texelFetch(g_diffuse, cs_id, 0);
		vec4 spec = texelFetch(g_specular, cs_id, 0);


		/*compute_paralel_light(light_data[1], normal, position, tex_norm.a, diff, spec, ambient, diffuse, specular);
		diff_sum += ambient + diffuse;
		spec_sum += specular;*--/

		vec3 vViewDir = normalize(g_vCameraPos - position);

		for (int i = 0; i < 12; i++) { DoPointLighting(i, normal, position, vViewDir, tex_norm.a, diffuse, specular); AccumDiffuse += diffuse; AccumSpecular += specular; }
		for (int i = 0; i < 12; i++) { DoSpotLighting( i, normal, position, vViewDir, tex_norm.a, diffuse, specular); AccumDiffuse += diffuse; AccumSpecular += specular; }
		//vec4 Ambient = clamp(AccumDiffuse.a, 0.0f, 1.0f) * diff.a * diff;

		//AccumDiffuse *= 20.0f;
		//AccumSpecular *= 8.0f;

		/*compute_paralel_light(light_data[0], normal, position, tex_norm.a, diff, spec, ambient, diffuse, specular);
		AccumDiffuse += diffuse;
		AccumSpecular += specular;*--/

		vec4 g_AmbientColorUp = vec4(0.013f, 0.015f, 0.050f, 1.0f);
		vec4 g_AmbientColorDown = vec4(0.0013f, 0.0015f, 0.0050f, 1.0f);
		float fAmbientBlend = 0.5f * normal.y + 0.5;
		vec4 Ambient = g_AmbientColorUp * fAmbientBlend + g_AmbientColorDown * (1 - fAmbientBlend);
		vec4 DiffuseAndAmbient = AccumDiffuse + Ambient;

		DiffuseAndAmbient *= diff;
		AccumSpecular *= diff * spec;

		//AccumDiffuse += Ambient;

		imageStore(g_spec_albedro, cs_id, AccumSpecular);//diff_sum
		imageStore(g_albedro, cs_id, DiffuseAndAmbient + AccumSpecular); //texelFetch(g_specular, cs_id, 0));// vec4(ExtractNormal(cs_id), 1.0f)); //texelFetch(g_diffuse, cs_id, 0));//vec4(ExtractNormal(cs_id), 1.0f));
		//vec4(normal, 1.0f));//vec4(0.5*normal + vec3(0.5f), 1.0f));//
		*/
	}
	//light_data[1].diffuse + light_data[0].diffuse);//
}
//WARNING DEPTH [0;1]!!!!