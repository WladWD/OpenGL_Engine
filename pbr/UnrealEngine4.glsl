//- Unreal Engine 4 shader for Substance Painter
//- ================================================
//-
//- Import from libraries.
import lib-sampler.glsl
import lib-pbr.glsl
import lib-alpha.glsl
//- Show back faces as there may be holes in front faces.
//: state cull_face off

//- Enable alpha blending
//: state blend over

//- Channels needed for metal/rough workflow are bound here.
//: param auto channel_basecolor
uniform sampler2D basecolor_tex;
//: param auto channel_roughness
uniform sampler2D roughness_tex;
//: param auto channel_metallic
uniform sampler2D metallic_tex;
//: param custom { "default": false, "label": "BruteForce" }
uniform bool BruteForce;

//--------------------------------------------------------------------------------------------------
vec3 pow3(vec3 a, float b)
{
return vec3(pow(a.x,b),pow(a.y,b),pow(a.z,b));
}
//--------------------------------------------------------------------------------------------------
vec3 mix3(vec3 a, vec3 b, float c)
{
return a * (1 - c) + c * b;
}
//--------------------------------------------------------------------------------------------------
 float radicalInverse_VdC(uint bits) 
 {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits); // / 0x100000000
 }
//--------------------------------------------------------------------------------------------------
 vec2 hammersley2d(uint i, uint N, uvec2 Random)
 {
	float X =  fract(float(i)/float(N) + float( Random.x & uint( 0xffff) ) / (1<<16) );
	float Y = float(uint(radicalInverse_VdC(i)) ^ Random.y) *  2.3283064365386963e-10;
    return vec2(X,Y);
 }
//--------------------------------------------------------------------------------------------------
uvec2 ScrambleTEA(uvec2 v)
{
	uint y 			= v[0];
	uint z 			= v[1];
	uint sum 		= uint(0);
	uint iCount 	= uint(4);
	 for(uint i = uint(0); i < iCount; ++i)
	{
		sum += uint(0x9e3779b9);
		y += (z << 4u) + 0xA341316Cu  ^ z + sum ^ (z >> 5u) + 0xC8013EA4u;
		z += (y << 4u) + 0xAD90777Du ^ y + sum ^ (y >> 5u) +  0x7E95761Eu;
	}
	return uvec2(y, z);
}
//--------------------------------------------------------------------------------------------------
float getLOD(float roughness)
{
 // return 3;
  return max((maxLod-4) * min(roughness *3,1),3);
}
//--------------------------------------------------------------------------------------------------
float getMipFB3(float roughness,float vdh,float ndh,float SamplesNum)
{
	float width = 500 ;
	float omegaS = 1 /  (SamplesNum *probabilityGGX(ndh, vdh, max(roughness,0.01)));
	float omegaP = 4.0 * 3.141592 / (6.0 * width * width ) ;
	return clamp (0.5 * log2 ( omegaS / omegaP ) , 3, maxLod );
}
//--------------------------------------------------------------------------------------------------
// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox( float Roughness, float NoV, float NoL )
{
	float a = Roughness * Roughness;
	float Vis_SmithV = NoL * ( NoV * ( 1 - a ) + a );
	float Vis_SmithL = NoV * ( NoL * ( 1 - a ) + a );
	return 0.5 / ( Vis_SmithV + Vis_SmithL );
}
//--------------------------------------------------------------------------------------------------
// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
vec3 F_Schlick( vec3 SpecularColor, float VoH )
{
	float Fc = pow( 1 - VoH, 5);					
	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	return min( 50.0 * SpecularColor.g, 1) * Fc + (1 - Fc) * SpecularColor;
}
//--------------------------------------------------------------------------------------------------
//- Compute the outgoing light radiance to the viewer's eye
	vec4 ComputeUE4BRDF(V2F inputs, vec3 diffColor, float metallic, float roughness, float occlusion, int SamplesNum)
{
  //- Get world space normal
	vec3 normal_vec = computeWSNormal(inputs.tex_coord, inputs.tangent, inputs.bitangent, inputs.normal);
	

  //- Double-sided normal if back faces are visible
  if (isBackFace()) 
  {
    normal_vec = -normal_vec;
  }

  vec3 eye_vec = normalize(camera_pos - inputs.position);
  float ndv = dot(eye_vec, normal_vec);

  //- Trick to remove black artefacts
  //- Backface ? place the eye at the opposite - removes black zones
  if (ndv < 0) 
  {
    eye_vec = reflect(eye_vec, normal_vec);
    ndv = abs(ndv);
  }

  //- Diffuse contribution
  vec3 diffuseIrradiance	= envIrradiance(normal_vec);
  vec3 contribE 			= occlusion * diffuseIrradiance * diffColor * (1.0 - metallic);
  vec3 specColor 			= mix3(vec3(0.08), diffColor, metallic);
  float lerp2uniform		= max(2*(roughness  - 0.5) ,0.0001);

  //- Specular contribution
 
	
  //- Create a local basis for BRDF work
  vec3 Tp = normalize(inputs.tangent - normal_vec*dot(inputs.tangent, normal_vec)); // local tangent
  vec3 Bp = normalize(inputs.bitangent - normal_vec*dot(inputs.bitangent, normal_vec)- Tp*dot(inputs.bitangent, Tp)); // local bitangent
  vec3 contribS = vec3(0.0);
  uvec2 Random = ScrambleTEA(uvec2(gl_FragCoord.xy ));

  //- Brute force sampling 
  if (BruteForce)
  {
  SamplesNum = 512;
  }

  for(int i = 0; i < SamplesNum; ++i)
  {
    vec2 Xi 	 = hammersley2d(uint(i), uint(SamplesNum),Random);
    vec3 Hn 	 = importanceSampleGGX(Xi,Tp,Bp,normal_vec,roughness);
    vec3 Ln 	 = -reflect(eye_vec,Hn);

    float ndl 	= max(1e-8, (dot(normal_vec, Ln)));
    float vdh 	= max(1e-8, dot(eye_vec, Hn));
    float ndh 	= max(1e-8, dot(normal_vec, Hn));
	float PDF	= (4 * vdh / ndh);
	 
	//Blend importance sampled Irradiance with Uniform distribution Irradiance when roughness --> 1 
	float mip = getMipFB3( roughness, vdh, ndh, SamplesNum);
	vec3 irr = envSampleLOD(Ln, mip);
	vec3 specularIrradiance = mix3(irr, diffuseIrradiance, lerp2uniform);
	contribS +=  specularIrradiance * F_Schlick(specColor, vdh ) * clamp(Vis_SmithJointApprox( roughness, ndv, ndl ) * PDF * ndl ,0,1);
  }
  // Remove occlusions on shiny reflections
  contribS *= mix(occlusion, 1.0, roughness) / float(SamplesNum);

  //- Emissive
  vec3 contribEm = emissive_intensity * texture2D(emissive_tex, inputs.tex_coord).rgb;

  //- Sum diffuse + spec + emissive
  return vec4(contribS + contribE, 1.0);
}

//- Shader entry point.
vec4 shade(V2F inputs)
{
  float glossiness = getRoughness(roughness_tex, inputs.tex_coord);
  vec4 baseColor = texture2D(basecolor_tex, inputs.tex_coord);
  alphaKill(inputs.tex_coord);
  vec3 metallic = texture2D(metallic_tex, inputs.tex_coord).xyz;
  float occlusion = getAO(inputs.tex_coord) * getShadowFactor();

  // Feed parameters for a physically based BRDF integration
  return vec4( ComputeUE4BRDF(inputs, baseColor.xyz, metallic.x, glossiness * glossiness, occlusion, nbSamples).rgb,1.0f);
}

//- Entry point of the shadow pass.
void shadeShadow(V2F inputs)
{
}