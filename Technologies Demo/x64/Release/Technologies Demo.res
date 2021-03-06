        џџ  џџ                  њ  (   T E S T   џџe       0         #version 440 core
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 out_v_pos;
	vec3 out_v_normal;
	vec2 out_v_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 proj_view_world;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 2)uniform vec4 diffuse;
layout (location = 3)uniform vec4 specular;
layout (location = 4)uniform vec4 emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;//sampler2D samplerCube
//uniform sampler2D g_normal;
//uniform sampler2D g_reflaction; in Alpha save - specular intensivity (reflaction intensyviti), in rgb - reflaction
////////////////////////////////////////////////////
layout (location = 6)uniform vec3 g_lightDir;//mast be normalize
layout (location = 7)uniform vec4 g_lightAmbient;
layout (location = 8)uniform vec4 g_lightDiffuse;
layout (location = 9)uniform vec4 g_lightSpecular;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = diffuse * g_lightAmbient * diffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), specular.a), 0.0);
	return specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.out_v_normal);
	vec3 viewDir  = normalize(g_cam_pos - vertex_out.out_v_pos);
	vec3 lightDir = normalize(-g_lightDir);

	//vec3 cb_vec = reflect(-viewDir, normal);
	//cb_vec.y = -cb_vec.y;
	vec4 tex_diff = texture(g_diffuse, vertex_out.out_v_tex);//cb_vec);

	if(tex_diff.a < 0.6f) discard;

	res_color = emissive;
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir)) * tex_diff;
	res_color += comp_spec(normal, viewDir, lightDir);

	res_color = vec4(res_color.rgb, 1.0f);
}
  У  (   T E S T   џџf       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 out_v_pos;
	vec3 out_v_normal;
	vec2 out_v_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 proj_view_world;
////////////////////////////////////////////////////
void main()
{
	vec4 pos = vec4(v_position, 1.0);
	vertex_out.out_v_pos = v_position;
	vertex_out.out_v_normal = v_normal;
	vertex_out.out_v_tex = v_tex;

	gl_Position = proj_view_world * pos;
} Z  ,   S H A D E R   џџg       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5)uniform vec3 g_lightDir;//mast by normalize
layout (location = 6)uniform vec4 g_lightDiffuse;
layout (location = 7)uniform vec4 g_lightSpecular;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = m_diffuse * g_lightDiffuse * m_diffuse.a * g_lightDiffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return m_diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), m_specular.a), 0.0);
	return m_specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 lightDir = normalize(-g_lightDir);

	res_color = m_emissive;
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir));
	res_color += comp_spec(normal, viewDir, lightDir);

	res_color = vec4(res_color.rgb, 1.0f);
}
    ,   S H A D E R   џџh       0         #version 440 core
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
}  
  ,   S H A D E R   џџi       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5)uniform vec3 g_lightDir;//mast by normalize
layout (location = 6)uniform vec4 g_lightDiffuse;
layout (location = 7)uniform vec4 g_lightSpecular;
////////////////////////////////////////////////////
layout (location = 8, binding = 0)uniform sampler2D g_diffuse;
layout (location = 9, binding = 1)uniform sampler2D g_specular;//only red chanel
layout (location = 10, binding = 2)uniform samplerCube g_reflaction;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = m_diffuse * g_lightDiffuse * m_diffuse.a * g_lightDiffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return m_diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), m_specular.a), 0.0);
	return m_specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 lightDir = normalize(-g_lightDir);
	vec3 reflactinDir = normalize(reflect(viewDir, normal));
	reflactinDir.y = -reflactinDir.y;

	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	float spec_val = texture(g_specular, vertex_out.w_tex).r;
	vec4 spec_tex_reflaction = texture(g_reflaction, reflactinDir);

	if(diff_tex_color.a < 0.6f)discard;

	res_color = m_emissive;
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir)) * diff_tex_color;
	res_color += comp_spec(normal, viewDir, lightDir) * spec_val * spec_tex_reflaction;//spec_val comp_spec(normal, viewDir, lightDir)

	res_color = vec4(res_color.rgb, 1.0f);
}
  I  ,   S H A D E R   џџj       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5)uniform vec3 g_lightDir;//mast by normalize
layout (location = 6)uniform vec4 g_lightDiffuse;
layout (location = 7)uniform vec4 g_lightSpecular;
////////////////////////////////////////////////////
layout (location = 8, binding = 0)uniform sampler2D g_diffuse;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = m_diffuse * g_lightDiffuse * m_diffuse.a * g_lightDiffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return m_diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), m_specular.a), 0.0);
	return m_specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 lightDir = normalize(-g_lightDir);

	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);

	if(diff_tex_color.a < 0.6f)discard;

	res_color = m_emissive;
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir)) * diff_tex_color;
	res_color += comp_spec(normal, viewDir, lightDir);

	res_color = vec4(res_color.rgb, 1.0f);
}
   X  ,   S H A D E R   џџk       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 m_tn;
layout (location = 3) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
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
	vertex_out.w_tangent = m_tn;
	vertex_out.w_tex = v_tex;
	vertex_out.w_eye_dir = v_position - g_cam_pos;

	gl_Position = g_proj_view * vec4(v_position, 1.0);
}[  ,   S H A D E R   џџl       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 g_proj_view;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5)uniform vec3 g_lightDir;//mast by normalize
layout (location = 6)uniform vec4 g_lightDiffuse;
layout (location = 7)uniform vec4 g_lightSpecular;
////////////////////////////////////////////////////
layout (location = 8, binding = 0)uniform sampler2D g_diffuse;
layout (location = 9, binding = 1)uniform sampler2D g_normal;
layout (location = 10, binding = 2)uniform sampler2D g_specular;//only red chanel
layout (location = 11, binding = 3)uniform samplerCube g_reflaction;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = m_diffuse * g_lightDiffuse * m_diffuse.a * g_lightDiffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return m_diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), m_specular.a), 0.0);
	return m_specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
vec3 ComputeBumpNormal(vec3 bump_map_normal, vec3 normal, vec3 tangent)
{
	vec3 Normal0 = normalize(normal);
	vec3 Tangent0 = normalize(tangent);
	Tangent0 = normalize(Tangent0 - (dot(Tangent0, Normal0) * Normal0));
	vec3 BiNormal0 = cross(Tangent0, Normal0);

	bump_map_normal = (bump_map_normal * 2.0f) - vec3(1.0f);
	bump_map_normal = normalize(bump_map_normal);
	vec3 Normal_Computed;
	mat3 TBN = mat3(Tangent0, BiNormal0, Normal0);
	Normal_Computed = TBN * bump_map_normal;
	Normal_Computed = normalize(Normal_Computed);
	return Normal_Computed;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 tangent   = normalize(vertex_out.w_tangent);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 lightDir = normalize(-g_lightDir);
	vec3 reflactinDir = normalize(reflect(viewDir, normal));
	reflactinDir.y = -reflactinDir.y;


	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	vec4 normalmap_normal = texture(g_normal, vertex_out.w_tex);
	float spec_val = texture(g_specular, vertex_out.w_tex).r;
	vec4 spec_tex_reflaction = texture(g_reflaction, reflactinDir);

	if(diff_tex_color.a < 0.6f)discard;

	normal = ComputeBumpNormal(normalmap_normal.xyz, normal, tangent);

	res_color = m_emissive;
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir)) * diff_tex_color;
	res_color += comp_spec(normal, viewDir, lightDir) * spec_val * spec_tex_reflaction;

	res_color = vec4(res_color.rgb, 1.0f);
}
 Ф  ,   S H A D E R   џџm       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 g_proj_view;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5)uniform vec3 g_lightDir;//mast by normalize
layout (location = 6)uniform vec4 g_lightDiffuse;
layout (location = 7)uniform vec4 g_lightSpecular;
////////////////////////////////////////////////////
layout (location = 8, binding = 0)uniform sampler2D g_diffuse;
layout (location = 9, binding = 1)uniform sampler2D g_normal;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = m_diffuse * g_lightDiffuse * m_diffuse.a * g_lightDiffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return m_diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), m_specular.a), 0.0);
	return m_specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
vec3 ComputeBumpNormal(vec3 bump_map_normal, vec3 normal, vec3 tangent)
{
	vec3 Normal0 = normalize(normal);
	vec3 Tangent0 = normalize(tangent);
	Tangent0 = normalize(Tangent0 - (dot(Tangent0, Normal0) * Normal0));
	vec3 BiNormal0 = cross(Tangent0, Normal0);

	bump_map_normal = (bump_map_normal * 2.0f) - vec3(1.0f);
	bump_map_normal = normalize(bump_map_normal);
	vec3 Normal_Computed;
	mat3 TBN = mat3(Tangent0, BiNormal0, Normal0);
	Normal_Computed = TBN * bump_map_normal;
	Normal_Computed = normalize(Normal_Computed);
	return Normal_Computed;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 tangent   = normalize(vertex_out.w_tangent);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 lightDir = normalize(-g_lightDir);


	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	vec4 normalmap_normal = texture(g_normal, vertex_out.w_tex);

	if(diff_tex_color.a < 0.6f)discard;

	normal = ComputeBumpNormal(normalmap_normal.xyz, normal, tangent);

	res_color = m_emissive;
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir)) * diff_tex_color;
	res_color += comp_spec(normal, viewDir, lightDir);

	res_color = vec4(res_color.rgb, 1.0f);
}
Y  ,   S H A D E R   џџn       0         #version 440 core
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform float InvOpticalDepthNLessOne;// = 1.0f/255.0f;
layout (location = 1)uniform float InvOpticalDepthN;//  = 1.0f/256.0f;
layout (location = 2)uniform float InnerRadius;//  = 6356.7523142;
layout (location = 3)uniform float OuterRadius;//  = 6356.7523142 * 1.0157313;
layout (location = 4)uniform float PI;//  = 3.1415159;
layout (location = 5)uniform float NumSamples;//  = 20;
layout (location = 6)uniform float fScale;//  = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);
layout (location = 7)uniform vec2 v2dRayleighMieScaleHeight;//  = vec2(0.25, 0.1);
////////////////////////////////////////////////////
layout (location = 8)uniform vec2 InvRayleighMieNLessOne;//  = vec2(1.0f/255.0f, 1.0f/127.0f);
layout (location = 9)uniform vec3 v3SunDir;//  = vec3( 0, 1, 0 );
//float ESun = 20.0f;
//float Kr = 0.0025f;
//float Km = 0.0010f;
layout (location = 10)uniform float KrESun;//  = 0.0025f * 20.0f;
layout (location = 11)uniform float KmESun;//  = 0.0010f * 20.0f;
layout (location = 12)uniform float Kr4PI;//  = 0.0025f * 4.0f * 3.1415159;
layout (location = 13)uniform float Km4PI;//  = 0.0010f * 4.0f * 3.1415159;
////////////////////////////////////////////////////
layout (location = 14)uniform float g;//  = -0.990;
layout (location = 15)uniform float g2;//  = (-0.990) * (-0.990);
layout (location = 16)uniform float fExposure;//  = -2.0;
////////////////////////////////////////////////////
layout (location = 17)uniform vec3 InvWavelength;
layout (location = 18)uniform vec3 WavelengthMie;
////////////////////////////////////////////////////
layout (location = 19)uniform float starIntensity;//  = 0.5f;
////////////////////////////////////////////////////
float HitOuterSphere( vec3 O, vec3 Dir ) 
{
	vec3 L = -O;

	float B = dot( L, Dir );
	float C = dot( L, L );
	float D = C - B * B; 
	float q = sqrt( OuterRadius * OuterRadius - D );
	float t = B;
	t += q;

	return t;
}

vec2 GetDensityRatio( float fHeight )
{
	const float fAltitude = (fHeight - InnerRadius) * fScale;
	return exp( (-fAltitude) / v2dRayleighMieScaleHeight.xy );
}

vec2 t( vec3 P, vec3 Px )
{

	vec2 OpticalDepth = vec2(0.0);

	vec3 v3Vector =  Px - P;
	float fFar = length( v3Vector );
	vec3 v3Dir = v3Vector / fFar;
			
	float fSampleLength = fFar / NumSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Dir * fSampleLength;
	P += v3SampleRay * 0.5f;
			
	for(int i = 0; i < NumSamples; i++)
	{
		float fHeight = length( P );
		OpticalDepth += GetDensityRatio( fHeight );
		P += v3SampleRay;
	}		

	OpticalDepth *= fScaledLength;
	return OpticalDepth;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 RayLeighTexture;
layout (location = 1) out vec4 MieTexture;
////////////////////////////////////////////////////
void main()
{
	vec2 Tex0 = (vertex_out.w_tex);
	 
	const vec3 v3PointPv = vec3( 0, InnerRadius + (1e-3), 0 );
	const float AngleY = (100.0 * Tex0.x * PI) / 180.0;
	const float AngleXZ = PI * Tex0.y;
	
	vec3 v3Dir;
	v3Dir.x = sin( AngleY ) * cos( AngleXZ  );
	v3Dir.y = cos( AngleY );
	v3Dir.z = sin( AngleY ) * sin( AngleXZ  );
	v3Dir = normalize( v3Dir );

	float fFarPvPa = HitOuterSphere( v3PointPv , v3Dir );
	vec3 v3Ray = v3Dir;

	vec3 v3PointP = v3PointPv;
	float fSampleLength = fFarPvPa / NumSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	v3PointP += v3SampleRay * 0.5f;
				
	vec3 v3RayleighSum = vec3(0);
	vec3 v3MieSum = vec3(0);
	
	for( int k = 0; k < NumSamples; k++ )
	{
		float PointPHeight = length( v3PointP );

		vec2 DensityRatio = GetDensityRatio( PointPHeight );
		DensityRatio *= fScaledLength;

		vec2 ViewerOpticalDepth = t( v3PointP, v3PointPv );
						
		float dFarPPc = HitOuterSphere( v3PointP, v3SunDir );
		vec2 SunOpticalDepth = t( v3PointP, v3PointP + (v3SunDir * dFarPPc) );

		vec2 OpticalDepthP = SunOpticalDepth.xy + ViewerOpticalDepth.xy;
		vec3 v3Attenuation = vec3(exp( (-Kr4PI * InvWavelength * OpticalDepthP.x) - (Km4PI * OpticalDepthP.y)));

		v3RayleighSum += DensityRatio.x * v3Attenuation;
		v3MieSum += DensityRatio.y * v3Attenuation;

		v3PointP += v3SampleRay;
	}

	vec3 RayLeigh = v3RayleighSum * KrESun;
	vec3 Mie = v3MieSum * KmESun;
	RayLeigh *= InvWavelength;
	Mie *= WavelengthMie;
	
	RayLeighTexture = vec4( RayLeigh, 1.0f);
	MieTexture = vec4( Mie, 1.0f );
}




   У  ,   S H A D E R   џџo       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
void main()
{
	vertex_out.w_pos = v_position;
	vertex_out.w_tex = v_tex;

	gl_Position = vec4(v_position, 1.0);
}
   ,   S H A D E R   џџp       0         #version 440 core
////////////////////////////////////////////////////
layout(early_fragment_tests) in;
////////////////////////////////////////////////////
#define msv 1.0f / (2800.0f * 128.0f * 100.0f)
////////////////////////////////////////////////////
in struct FS_INPUT
{
	vec3 w_pos;
	vec2 w_tex0;
	vec3 w_tex1;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view_world;
layout (location = 1) uniform float InvOpticalDepthNLessOne;// = 1.0f/255.0f;
layout (location = 2) uniform float InvOpticalDepthN;//  = 1.0f/256.0f;
layout (location = 3) uniform float InnerRadius;//  = 6356.7523142;
layout (location = 4) uniform float OuterRadius;//  = 6356.7523142 * 1.0157313;
layout (location = 5) uniform float PI;//  = 3.1415159;
layout (location = 6) uniform float NumSamples;//  = 20;
layout (location = 7) uniform float fScale;//  = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);
layout (location = 8) uniform vec2 v2dRayleighMieScaleHeight;//  = vec2(0.25, 0.1);
////////////////////////////////////////////////////
layout (location = 9) uniform vec2 InvRayleighMieNLessOne;//  = vec2(1.0f/255.0f, 1.0f/127.0f);
layout (location = 10) uniform vec3 v3SunDir;//  = vec3( 0, 1, 0 );
//float ESun = 20.0f;
//float Kr = 0.0025f;
//float Km = 0.0010f;
layout (location = 11) uniform float KrESun;//  = 0.0025f * 20.0f;
layout (location = 12) uniform float KmESun;//  = 0.0010f * 20.0f;
layout (location = 13) uniform float Kr4PI;//  = 0.0025f * 4.0f * 3.1415159;
layout (location = 14) uniform float Km4PI;//  = 0.0010f * 4.0f * 3.1415159;
////////////////////////////////////////////////////
layout (location = 15) uniform float g;//  = -0.990;
layout (location = 16) uniform float g2;//  = (-0.990) * (-0.990);
layout (location = 17) uniform float fExposure;//  = -2.0;
////////////////////////////////////////////////////
layout (location = 18) uniform vec3 InvWavelength;
layout (location = 19) uniform vec3 WavelengthMie;
////////////////////////////////////////////////////
layout (location = 20) uniform float starIntensity;//  = 0.5f;
////////////////////////////////////////////////////
layout (location = 21, binding = 0) uniform sampler2D RayLeighTexture;
layout (location = 22, binding = 1) uniform sampler2D MieTexture;
layout (location = 23, binding = 2) uniform sampler2D StarTexture;//sampler2D samplerCube
////////////////////////////////////////////////////
float getRayleighPhase(float fCos2)
{
	return 0.75f * (1.0f + fCos2);
}

float getMiePhase(float fCos, float fCos2)
{
	vec3 v3HG;
	v3HG.x = 1.5f * ( (1.0f - g2) / (2.0f + g2) );
	v3HG.y = 1.0f + g2;
	v3HG.z = 2.0f * g;
	return (v3HG.x * (1.0 + fCos2)) / (pow(v3HG.y - (v3HG.z * fCos), 1.5));
}

vec3 HDR( vec3 LDR)
{
	return vec3(1.0f) - exp( fExposure * LDR );
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 SkyColor;
////////////////////////////////////////////////////
void main()
{
	float fCos = -clamp(dot( normalize(v3SunDir), fs_input.w_tex1 ) / length( fs_input.w_tex1 ), 0.0f, 1.0f);
	float fCos2 = fCos * fCos;
	
	vec3 v3RayleighSamples = texture(RayLeighTexture, fs_input.w_tex0).xyz;
	vec3 v3MieSamples = texture(MieTexture, fs_input.w_tex0).xyz;
	//v3MieSamples.yz = texture(MieTexture, fs_input.w_tex0).xy;
	//v3MieSamples.x = v3RayleighSamples.w;

	vec3 Color;
	Color.rgb = getRayleighPhase(fCos2) * v3RayleighSamples.rgb + getMiePhase(fCos, fCos2) * v3MieSamples.rgb;
	Color.rgb = HDR( Color.rgb );
	
	// Hack Sky Night Color
	Color.rgb += max(vec3(0.0f), (vec3(1.0f) - Color.rgb)) * vec3( 0.05, 0.05, 0.1 ); 

	vec2 m_star_tex1 = fs_input.w_tex1.xz * msv * 1.4f;// * 2.0f;
	vec2 m_star_tex2 = fs_input.w_tex0 * vec2(3.2f, 6.4f);//6.0f);
	vec4 star_tex1 = texture(StarTexture, m_star_tex1);// * 1.2f;//textureLod texture
	vec4 star_tex2 = texture(StarTexture, m_star_tex2) ;//* 2.2f;

	float hv = clamp(fs_input.w_tex1.y * msv + 0.2, 0.0f, 1.0f);
	float t_a = pow(hv, 0.23f);//hv;//sqrt(hv);//pow(hv, 2.0f);

	t_a = t_a >= 0.98f ? t_a : t_a * 0.021f;

	vec4 m_star_tex = (star_tex1 * t_a + (1.0f - t_a) * star_tex2) * 1.8f;

	m_star_tex = pow(m_star_tex, vec4(4.00041f));// * m_star_tex;

	SkyColor = vec4( Color.rgb, 1.0f ) + m_star_tex * starIntensity;//texture(StarTexture, fs_input.w_tex0) * starIntensity;  //fs_input.w_tex0 w_tex1 textureLod  texture
	SkyColor.a = 1.0f;
}



   ,   S H A D E R   џџq       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct FS_INPUT
{
	vec3 w_pos;
	vec2 w_tex0;
	vec3 w_tex1;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view_world;
////////////////////////////////////////////////////
void main()
{
	fs_input.w_pos = v_position;
	fs_input.w_tex0 = v_tex;
	fs_input.w_tex1 = v_position;//-v_position

	gl_Position = g_proj_view_world * vec4(v_position, 1.0);
	gl_Position.z = gl_Position.w;
}
Ж  ,   S H A D E R   џџr       0         #version 440 core
////////////////////////////////////////////////////
in struct FS_INPUT
{
	vec3 w_pos;
	vec2 w_tex;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view_world;
layout (location = 1) uniform mat4 g_world;
layout (location = 2) uniform float g_alpha;
////////////////////////////////////////////////////
layout (location = 3, binding = 0) uniform sampler2D MTexture;
////////////////////////////////////////////////////
layout (location = 0) out vec4 Color;
////////////////////////////////////////////////////
void main()
{
	Color = texture(MTexture, fs_input.w_tex);
	Color.a *= g_alpha;
}



  и  ,   S H A D E R   џџs       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct FS_INPUT
{
	vec3 w_pos;
	vec2 w_tex;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view_world;
layout (location = 1) uniform mat4 g_world;
layout (location = 2) uniform float g_alpha;
////////////////////////////////////////////////////
void main()
{
	vec4 mpos = vec4(v_position, 1.0f);
	fs_input.w_pos = (g_world * mpos).xyz;
	fs_input.w_tex = v_tex;

	gl_Position = g_proj_view_world * mpos;
	gl_Position.z = gl_Position.w;
}
  ,   S H A D E R   џџt       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
//////////////////////////////////////////////////// 
//(2800.0f * 128.0f * 100.0f)
#define MCV (2800.0f * 128.0f * 100.0f)
#define FOG_START (280.0f * 128.0f)//128.0f * 60.0f)
#define FOR_RANGE (2800.0f * 128.0f  )//* 128.0f * 80.0f)
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 gWorld;
layout (location = 1)uniform mat4 gProjView;
layout (location = 2)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 3)uniform vec4 m_diffuse;
layout (location = 4)uniform vec4 m_specular;
layout (location = 5)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 6)uniform vec3 g_lightDir;//mast by normalize
layout (location = 7)uniform vec4 g_lightDiffuse;
layout (location = 8)uniform vec4 g_lightSpecular;
layout (location = 9)uniform float g_StarIntensity;
////////////////////////////////////////////////////
layout (location = 10, binding = 0)uniform sampler2D g_diffuse;
layout (location = 11, binding = 1)uniform sampler2D g_normal;
////////////////////////////////////////////////////
vec4 comp_ambient()
{
	vec4 ambient = m_diffuse * g_lightDiffuse * m_diffuse.a * g_lightDiffuse.a;
	return vec4(ambient.rgb, 1.0f);
}

vec4 comp_diffuse(vec3 normal, vec3 lightDir)
{
	float NdotL = max(dot(normal, lightDir), 0.0);
	return m_diffuse * g_lightDiffuse * NdotL;
}

vec4 comp_spec(vec3 normal, vec3 viewDir, vec3 lightDir)
{
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), m_specular.a), 0.0);
	return m_specular * g_lightSpecular * RdotVpow;
}
////////////////////////////////////////////////////
vec3 ComputeBumpNormal(vec3 bump_map_normal, vec3 normal, vec3 tangent)
{
	vec3 Normal0 = normalize(normal);
	vec3 Tangent0 = normalize(tangent);
	Tangent0 = normalize(Tangent0 - (dot(Tangent0, Normal0) * Normal0));
	vec3 BiNormal0 = cross(Tangent0, Normal0);

	bump_map_normal = (bump_map_normal * 2.0f) - vec3(1.0f);
	bump_map_normal = normalize(bump_map_normal);
	vec3 Normal_Computed;
	mat3 TBN = mat3(Tangent0, BiNormal0, Normal0);
	Normal_Computed = TBN * bump_map_normal;
	Normal_Computed = normalize(Normal_Computed);
	return Normal_Computed;
}
////////////////////////////////////////////////////
vec3 ColorFog(vec3 mcolor, vec3 pos)
{
	float dist = length(pos.xz);
	float t = clamp(((dist - FOG_START) / FOR_RANGE), 0.0f, 1.0f);
	return mcolor * (1.0f - t) + g_lightDiffuse.rgb * t;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 lightDir = normalize(-g_lightDir);


	//dvec2 mtex = vertex_out.w_posdw.xy;//dvec2(0.5f)*(vertex_out.w_posdw.xz) + dvec2(0.5f);

	//mtex *= 2.0;//MCV;
	//mtex = mtex - dvec2(int(mtex.x), int(mtex.y));

	//vec4 diff_tex_color = texture(g_diffuse, mtex * 1200000);//vertex_out.w_pos.xz );//vertex_out.w_tex);
	//vec4 normalmap_normal = texture(g_normal, vertex_out.w_tex);

	//if(diff_tex_color.a < 0.6f)discard;

	//normal = ComputeBumpNormal(normalmap_normal.xyz, vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f));

	res_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	res_color += (comp_ambient() + comp_diffuse(normal, lightDir));// * diff_tex_color;
	res_color += comp_spec(normal, viewDir, lightDir);
	//- ivec2(vertex_out.w_tex)
	res_color = vec4(ColorFog(res_color.rgb, vertex_out.w_pos), 1.0f);//vec4(res_color.rgb, 1.0f);
}
№  ,   S H A D E R   џџu       0         #version 440 core
////////////////////////////////////////////////////
#define OFFSET 10.0f
#define REPEAD ( 64.0f)
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 gWorld;
layout (location = 1)uniform mat4 gProjView;
layout (location = 2)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
void main()
{
	vec4 pos = gWorld * vec4(v_position, 1.0);
	vertex_out.w_pos = pos.xyz;
	//vertex_out.w_posdw = (gWorld * vec4(v_tex, 0.0f, 1.0)).xyy;//pos.xyz;
	vertex_out.w_eye_dir = pos.xyz - g_cam_pos;
	vertex_out.w_tex = (gWorld * vec4(v_tex, 0.0f, 1.0)).xy ;//* REPEAD;

	pos.y -= OFFSET;
	gl_Position = gProjView * pos;
	gl_Position.z = gl_Position.w;
}  ,   S H A D E R   џџv       0         #version 440 core
////////////////////////////////////////////////////
layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct FS_INPUT
{
	vec3 w_pos;
	vec2 TexCoord;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view;
layout (location = 1) uniform vec3 g_offset_vec;
layout (location = 2) uniform vec3 g_cloud_color;
layout (location = 3) uniform float CloudCover;
layout (location = 4) uniform float CloudSharpness;
////////////////////////////////////////////////////
layout (location = 5, binding = 0) uniform sampler2D CloudTexture;
////////////////////////////////////////////////////
layout (location = 0) out vec4 SkyColor;
////////////////////////////////////////////////////
vec4 mSample(vec2 tex)
{
    vec4 perlin =	texture(CloudTexture,		vec2(tex)		  +  g_offset_vec.xy);
    perlin +=		texture(CloudTexture,		vec2(tex*2.0f)  +  g_offset_vec.xy)/2.0f;
    perlin +=		texture(CloudTexture,		vec2(tex*4.0f)  +  g_offset_vec.xy)/4.0f;
    perlin +=		texture(CloudTexture,		vec2(tex*8.0f)  +  g_offset_vec.xy)/8.0f;
    perlin +=		texture(CloudTexture,		vec2(tex*16.0f) +  g_offset_vec.xy)/16.0f;
    perlin +=		texture(CloudTexture,		vec2(tex*32.0f) +  g_offset_vec.xy)/32.0f;
	//perlin +=		texture(CloudTexture,		vec2(tex*64.0f) +  g_offset_vec.xy)/64.0f;
    //perlin +=		texture(CloudTexture,		vec2(tex*128.0f) +  g_offset_vec.xy)/128.0f;

	float nFinal = perlin.r;
	
	float c = CloudCover - nFinal;
    if (c < 0) c = 0;
 
    float CloudDensity = 1.0 - pow(CloudSharpness,c);
    vec4 CloudColor = vec4(CloudDensity);
	CloudColor.a = pow(CloudDensity, 70.8f);//10.81f);
	return CloudColor;
}
////////////////////////////////////////////////////
void main()
{
	SkyColor  = mSample(fs_input.TexCoord);
	SkyColor.rgb *= g_cloud_color;
	SkyColor.rgb = pow(SkyColor.rgb, vec3(1.2f));
}



 X  ,   S H A D E R   џџw       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct FS_INPUT
{
	vec3 w_pos;
	vec2 TexCoord;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view;
////////////////////////////////////////////////////
void main()
{
	fs_input.w_pos = v_position;
	fs_input.TexCoord = v_tex;

	gl_Position = g_proj_view * vec4(v_position, 1.0);
	gl_Position.z = gl_Position.w;
}
  ,   S H A D E R   џџx       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
////////////////////////////////////////////////////
in struct FS_INPUT
{
	vec3 w_pos;
	vec2 TexCoord;
} fs_input;
////////////////////////////////////////////////////
layout (location = 1) uniform float CloudCover;
layout (location = 2) uniform float CloudSharpness;
layout (location = 3) uniform vec3 m_offset;
////////////////////////////////////////////////////
layout (location = 4, binding = 0) uniform sampler3D NoiseTexture;
////////////////////////////////////////////////////
layout (location = 0) out vec4 CloudColor;
////////////////////////////////////////////////////
void main()
{
	vec3 offset = m_offset;//m_dir * time;
    vec4 perlin =	texture(NoiseTexture,		vec3(fs_input.TexCoord, 0.0f)		  +  offset);
    perlin +=		texture(NoiseTexture,		vec3((fs_input.TexCoord)*2.0f, 0.0f)  +  offset)/2.0f;
    perlin +=		texture(NoiseTexture,		vec3((fs_input.TexCoord)*4.0f, 0.0f)  +  offset)/4.0f;
    perlin +=		texture(NoiseTexture,		vec3((fs_input.TexCoord)*8.0f, 0.0f)  +  offset)/8.0f;
    perlin +=		texture(NoiseTexture,		vec3((fs_input.TexCoord)*16.0f, 0.0f) +  offset)/16.0f;
    perlin +=		texture(NoiseTexture,		vec3((fs_input.TexCoord)*32.0f, 0.0f) +  offset)/32.0f;

	float nFinal = perlin.r;
	
	float c = CloudCover - nFinal;
    if (c < 0) c = 0;
 
    float CloudDensity = 1.0 - pow(CloudSharpness,c);
    CloudColor = vec4(CloudDensity);
	CloudColor.a = pow(CloudDensity, 10.81f);
}



 "  ,   S H A D E R   џџy       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_tex;
////////////////////////////////////////////////////
out struct FS_INPUT
{
	vec3 w_pos;
	vec2 TexCoord;
} fs_input;
////////////////////////////////////////////////////
layout (location = 0) uniform mat4 g_proj_view;
void main()
{
	fs_input.w_pos = v_position;
	fs_input.TexCoord = v_tex;

	gl_Position = g_proj_view * vec4(v_position, 1.0);
	gl_Position.z = gl_Position.w;
}
  
  ,   S H A D E R   џџz       0         #version 440 core
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

	width *= 2.8f;//4.0f;
	height *= 3.0f;//4.0f;//3.0f - cur

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
     ,   S H A D E R   џџ{       0         #version 440 core
////////////////////////////////////////////////////
////////////////////////////////////////////////////
in vec2 ps_in_tex;
in vec3 ps_in_pos;
in flat uint ps_in_tex_id;
////////////////////////////////////////////////////
layout (location = 5)uniform vec4 g_SunLight;
layout (location = 6)uniform vec4 g_MoonLight;
layout (location = 7, binding = 0)uniform sampler2DArray g_drop_array;
////////////////////////////////////////////////////
layout (location = 0) out vec4 res_color;
////////////////////////////////////////////////////
void main()
{//vec4(1.0f);//
	vec4 tex_color = textureLod(g_drop_array, vec3(ps_in_tex, ps_in_tex_id), 0.0) + textureLod(g_drop_array, vec3(ps_in_tex, (ps_in_tex_id+5)%370), 0.0);//textureLod texture
	
	vec4 tex_color_ms = textureLod(g_drop_array, vec3(ps_in_tex, (ps_in_tex_id+3)%370), 0.0);
	tex_color = tex_color + tex_color * tex_color_ms;
	vec3 color_drop = vec3(g_MoonLight.rgb * g_MoonLight.a + g_SunLight.rgb * (1.0f-g_MoonLight.a));//vec3(1.0f) *( g_SunLight.rgb * g_SunLight.a + g_MoonLight.rgb * g_MoonLight.a );
	//color_drop *= pow(tex_color.r, 0.0000004f);
	res_color = vec4(color_drop, 0.13f * pow(tex_color.r, 1.00000984749f));//pow(tex_color.r, 0.4749f)); //0.1 0.27f
	//vec4(color_drop, 0.18f*pow(tex_color.r, 0.27f)); ----- // ёэхуююс№рчэћщ фюцфќ
	// 0.1f*0.9f*0.27f);

	//vec4(color_drop, 0.127f * pow(tex_color.r, 1.00000984749f)); - best 
}
//vec4(vec3(float(ps_in_tex_id) / 370.0f, 0.0f, 0.0f), 0.8f* tex_color.r);//
//0.2f*0.9f*0.27f
// 0.2f*0.18f* ~  ,   S H A D E R   џџ|       0         #version 440 core
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
  v  ,   S H A D E R   џџ}       0         #version 440 core
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
    
}*/    ,   S H A D E R   џџ~       0         #version 440 core
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
} р  ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
//#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
//#endif
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	
	//normal = vec3(0.5f)*normal + vec3(0.5f);
	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));
	out_color_1 = m_diffuse;// * (m_diffuse.a + 1.0f);// + m_emissive;
	out_color_1.a = m_diffuse.a;
	//out_color_1.a = 1.0f;
	out_color_2 = m_specular;
	out_color_2.a = 1.0f;///= 255.0f;
}
  ,   S H A D E R   џџ       0         #version 440 core
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
}  i  ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
#endif
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);

	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
		
#ifndef USE_NO_CLIP_ALPHA 
if(diff_tex_color.a < 0.6f)discard;//1 - pixel veisible, 0 - pixel not visible
#endif

#ifdef USE_NO_CLIP_ALPHA 
//if(!gl_FrontFacing)normal = -normal;
#endif

	//normal = vec3(0.5f) * normal + vec3(0.5f);
	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));
	out_color_1 = m_diffuse * diff_tex_color;// * (m_diffuse.a + 1.0f);// + m_emissive;
	out_color_1.a = m_diffuse.a;
	//out_color_1.a = 1.0f;
	out_color_2 = m_specular;
	out_color_2.a = 1.0f;///= 255.0f;
}
   !  ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
#endif
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;
layout (location = 6, binding = 1)uniform sampler2D g_specular;//only red chanel
layout (location = 7, binding = 2)uniform samplerCube g_reflaction;
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 reflactinDir = normalize(reflect(viewDir, normal));
	reflactinDir.y = -reflactinDir.y;

	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	float spec_val = texture(g_specular, vertex_out.w_tex).r;
	vec4 spec_tex_reflaction = texture(g_reflaction, reflactinDir);
	vec4 diff_tex_reflaction = textureLod(g_reflaction, reflactinDir, float(textureQueryLevels(g_reflaction)) * 0.65f);

#ifndef USE_NO_CLIP_ALPHA 
if(diff_tex_color.a < 0.6f)discard;
#endif

#ifdef USE_NO_CLIP_ALPHA 
//if(!gl_FrontFacing)normal = -normal;
#endif

	//normal = vec3(0.5f)*normal + vec3(0.5f);
	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));
	//out_color_1 = //mix(m_diffuse * diff_tex_color , spec_tex_reflaction * spec_val * m_specular, spec_val);//
	//m_diffuse * diff_tex_color + spec_tex_reflaction * spec_val * m_specular;// + m_emissive;

	float diff_intensity = spec_val * 0.5f;
	float spec_intensity = spec_val * 0.75f;

	vec4 diff_color = m_diffuse * diff_tex_color;// * (m_diffuse.a + 1.0f);//
	vec4 spec_color = m_specular;
	out_color_1 = mix(diff_color, diff_color * diff_tex_reflaction, diff_intensity);
	out_color_1.a = m_diffuse.a;
	//out_color_1 = mix(out_color_1, out_color_1 + spec_color * spec_tex_reflaction, spec_intensity);

	//out_color_1.a = spec_val;
	out_color_2 = m_specular;
	out_color_2.a = spec_val;///= 255.0f;
}
   е  ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
#endif
////////////////////////////////////////////////////
in struct VertexOut
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
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;
layout (location = 6, binding = 1)uniform sampler2D g_specular;//only red chanel
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);

	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	float spec_val = texture(g_specular, vertex_out.w_tex).r;

#ifndef USE_NO_CLIP_ALPHA 
if(diff_tex_color.a < 0.6f)discard;
#endif


#ifdef USE_NO_CLIP_ALPHA 
//if(!gl_FrontFacing)normal = -normal;
#endif


	//normal = vec3(0.5f)*normal + vec3(0.5f);
	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));
	out_color_1 = m_diffuse * diff_tex_color;// * (m_diffuse.a + 1.0f);// + m_emissive;
	out_color_1.a = m_diffuse.a;
	//out_color_1.a = spec_val;
	out_color_2 = m_specular;
	out_color_2.a = spec_val;// /= 255.0f;
}
   b  ,   S H A D E R   џџ       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_tangent;
layout (location = 3) in vec2 v_tex;
////////////////////////////////////////////////////
out struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
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
	vertex_out.w_tangent = v_tangent;
	vertex_out.w_tex = v_tex;
	vertex_out.w_eye_dir = v_position - g_cam_pos;

	gl_Position = g_proj_view * vec4(v_position, 1.0);
}  :  ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
#endif
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 g_proj_view;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;
layout (location = 6, binding = 1)uniform sampler2D g_normal;
layout (location = 7, binding = 2)uniform sampler2D g_specular;//only red chanel
layout (location = 8, binding = 3)uniform samplerCube g_reflaction;
////////////////////////////////////////////////////
vec3 ComputeBumpNormal(vec3 bump_map_normal, vec3 normal, vec3 tangent)
{
	vec3 Normal0 = normalize(normal);
	vec3 Tangent0 = normalize(tangent);
	Tangent0 = normalize(Tangent0 - (dot(Tangent0, Normal0) * Normal0));
	vec3 BiNormal0 = cross(Tangent0, Normal0);

	bump_map_normal = (bump_map_normal * 2.0f) - vec3(1.0f);
	bump_map_normal = normalize(bump_map_normal);
	vec3 Normal_Computed;
	mat3 TBN = mat3(Tangent0, BiNormal0, Normal0);
	Normal_Computed = TBN * bump_map_normal;
	Normal_Computed = normalize(Normal_Computed);
	return Normal_Computed;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r32g32b32a32  a - shininess(specular power) //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 tangent   = normalize(vertex_out.w_tangent);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);
	vec3 reflactinDir = normalize(reflect(viewDir, normal));
	reflactinDir.y = -reflactinDir.y;


	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	vec4 normalmap_normal = texture(g_normal, vertex_out.w_tex);
	float spec_val = texture(g_specular, vertex_out.w_tex).r;
	vec4 spec_tex_reflaction = texture(g_reflaction, reflactinDir);
	vec4 diff_tex_reflaction = textureLod(g_reflaction, reflactinDir, float(textureQueryLevels(g_reflaction)) * 0.6f);

#ifndef USE_NO_CLIP_ALPHA 
if(diff_tex_color.a < 0.6f)discard;
#endif

#ifdef USE_NO_CLIP_ALPHA 
//if(!gl_FrontFacing)normal = -normal;
#endif

	normal = ComputeBumpNormal(normalmap_normal.xyz, normal, tangent);
	//normal = vec3(0.5f)*normal + vec3(0.5f);

	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));

	vec4 diff_color = m_diffuse * diff_tex_color;//* ( 1.0f + m_diffuse)
	vec4 spec_color = m_specular;

	float diff_intensity = spec_val * 0.55f;//clamp(dot(diff_color.rgb, diff_color.rgb), 0.0f, 1.0f);//0.45f;
	float spec_intensity = spec_val * 0.75f;

	out_color_1 = mix(diff_color, diff_color * diff_tex_reflaction, diff_intensity);
	out_color_1.a = m_diffuse.a;
	//out_color_1 = mix(out_color_1, out_color_1 + spec_color * spec_tex_reflaction, spec_intensity);

	//out_color_1 = mix(m_diffuse * diff_tex_color , spec_tex_reflaction * spec_val * m_specular, spec_val);//
	//m_diffuse * diff_tex_color + spec_tex_reflaction * spec_val * m_specular;// + m_emissive;
	//out_color_1.a = spec_val;
	out_color_2 = mix(spec_color, spec_color * spec_tex_reflaction, spec_intensity);//m_specular;
	out_color_2.a = spec_val;///= 255.0f;
}
  K  ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
#endif
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 g_proj_view;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;
layout (location = 6, binding = 1)uniform sampler2D g_normal;
layout (location = 7, binding = 2)uniform sampler2D g_specular;//only red chanel
////////////////////////////////////////////////////
vec3 ComputeBumpNormal(vec3 bump_map_normal, vec3 normal, vec3 tangent)
{
	vec3 Normal0 = normalize(normal);
	vec3 Tangent0 = normalize(tangent);
	Tangent0 = normalize(Tangent0 - (dot(Tangent0, Normal0) * Normal0));
	vec3 BiNormal0 = cross(Tangent0, Normal0);

	bump_map_normal = (bump_map_normal * 2.0f) - vec3(1.0f);
	bump_map_normal = normalize(bump_map_normal);
	vec3 Normal_Computed;
	mat3 TBN = mat3(Tangent0, BiNormal0, Normal0);
	Normal_Computed = TBN * bump_map_normal;
	Normal_Computed = normalize(Normal_Computed);
	return Normal_Computed;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 tangent   = normalize(vertex_out.w_tangent);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);


	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	vec4 normalmap_normal = texture(g_normal, vertex_out.w_tex);
	float spec_val = texture(g_specular, vertex_out.w_tex).r;

#ifndef USE_NO_CLIP_ALPHA 
if(diff_tex_color.a < 0.6f)discard;
#endif

#ifdef USE_NO_CLIP_ALPHA 
//if(!gl_FrontFacing)normal = -normal;
#endif

	normal = ComputeBumpNormal(normalmap_normal.xyz, normal, tangent);
	//normal = vec3(0.5f)*normal + vec3(0.5f);

	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));
	out_color_1 = m_diffuse * diff_tex_color;// * (m_diffuse.a + 1.0f);// + m_emissive;
	out_color_1.a = m_diffuse.a;
	//out_color_1.a = spec_val;
	out_color_2 = m_specular;
	out_color_2.a = spec_val;///= 255.0f;
}
   ,   S H A D E R   џџ       0         //#version 440 core
////////////////////////////////////////////////////
#ifdef NOT_USE_CLIP_ALPHA
layout(early_fragment_tests) in;
#endif
////////////////////////////////////////////////////
in struct VertexOut
{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec3 w_eye_dir;
	vec2 w_tex;
} vertex_out;
////////////////////////////////////////////////////
layout (location = 0)uniform mat4 g_proj_view;
layout (location = 1)uniform vec3 g_cam_pos;
////////////////////////////////////////////////////
layout (location = 2)uniform vec4 m_diffuse;
layout (location = 3)uniform vec4 m_specular;
layout (location = 4)uniform vec4 m_emissive;
////////////////////////////////////////////////////
layout (location = 5, binding = 0)uniform sampler2D g_diffuse;
layout (location = 6, binding = 1)uniform sampler2D g_normal;
////////////////////////////////////////////////////
vec3 ComputeBumpNormal(vec3 bump_map_normal, vec3 normal, vec3 tangent)
{
	vec3 Normal0 = normalize(normal);
	vec3 Tangent0 = normalize(tangent);
	Tangent0 = normalize(Tangent0 - ((dot(Tangent0, Normal0)) * Normal0));
	vec3 BiNormal0 = normalize(cross(Tangent0, Normal0));

	//Tangent0 = normalize(cross(Normal0, BiNormal0));

	bump_map_normal = (bump_map_normal * 2.0f) - vec3(1.0f);
	bump_map_normal = normalize(bump_map_normal);
	vec3 Normal_Computed;
	mat3 TBN = (mat3(Tangent0, BiNormal0, Normal0));//transpose
	Normal_Computed = TBN * bump_map_normal;
	Normal_Computed = normalize(Normal_Computed);
	return Normal_Computed;
}
////////////////////////////////////////////////////
layout (location = 0) out vec4 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;
////////////////////////////////////////////////////
void main()
{
	vec3 normal   = normalize(vertex_out.w_normal);
	vec3 tangent   = normalize(vertex_out.w_tangent);
	vec3 viewDir  = normalize(-vertex_out.w_eye_dir);


	vec4 diff_tex_color = texture(g_diffuse, vertex_out.w_tex);
	vec4 normalmap_normal = texture(g_normal, vertex_out.w_tex);

#ifndef USE_NO_CLIP_ALPHA 
if(diff_tex_color.a < 0.6f)discard;
#endif

#ifdef USE_NO_CLIP_ALPHA 
//if(!gl_FrontFacing)normal = -normal;
#endif

	normal = ComputeBumpNormal(normalmap_normal.xyz, normal, tangent);
	//normal = vec3(0.5f)*normal + vec3(0.5f);
	out_color_0.xyz = normal;
	out_color_0.a = m_specular.a;
	//normal.z = sqrt(1 - dot(vec3(out_color_0.xy, 0.0f)));
	//out_color_1 = vec4(normal, 1.0f);
	out_color_1 = m_diffuse * diff_tex_color;// * (m_diffuse.a + 1.0f);// + m_emissive;
	out_color_1.a = m_diffuse.a;
	out_color_2 = m_specular;
	out_color_2.a = 1.0f;///= 255.0f;
}
ј  ,   S H A D E R   џџ       0         яЛП#version 440 core
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
}Л`  ,   S H A D E R   џџ       0         яЛП#version 440 core
//#pragma debug(on)
////////////////////////////////////////////////////
#define MAX_NUM_SHADOWCASTING_SPOTS 16
#define MAX_NUM_SHADOWCASTING_POINTS 16
////////////////////////////////////////////////////
#define POINT_LIGHT_SHADOW_CONST 0.00008f
////////////////////////////////////////////////////
#define TILE_RES 32
////////////////////////////////////////////////////
#define FLT_MAX         3.402823466e+38F
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
	//sampler2DArrayShadowт();

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
void compute_paralel_light(ParalelLight data_light, vec3 normal, vec3 position, float spec_pow, vec4 diff, vec4 spec, out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient = vec4(0.0f);
	diffuse = vec4(0.0f);
	specular = vec4(0.0f);

	vec3 li_dir = normalize(data_light.direction);

	ambient = diff * diff.a * data_light.diffuse * data_light.diffuse.a;
	ambient.a = 1.0f;

	diffuse = diff * data_light.diffuse * max(0.0f, dot(normal, -li_dir));// *0.34f;
	diffuse.a = 1.0f;

	vec3 r = normalize(reflect(li_dir, normal));
	vec3 v = normalize(g_vCameraPos - position);

	specular = spec * data_light.specular * spec.a * pow(max(0.0f, dot(v, r)), spec_pow) * 0.6f;
	specular.a = spec.a;
}
////////////////////////////////////////////////////
void DoPointLighting(in int nLightIndex, in vec3 normal, in vec3 position, in vec3 vViewDir, in float spec_pow,
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
		LightColorDiffuseResult = color * fFalloff * clamp( dot(normal, toLightDir), 0.0f, 1.0f) * color.a;
		//LightColorDiffuseResult.a = color.a;

		vec3 vHalfAngle = normalize(vViewDir + toLightDir);//blinn lighting
		LightColorSpecularResult = color * fFalloff * pow( clamp( dot(vHalfAngle, normal), 0.0f, 1.0f), spec_pow ) * pow(color.a, 1.8f);//spec_pow or 8

		float smShadow = DoPointShadow(nLightIndex, position);

		LightColorDiffuseResult *= smShadow;
		LightColorSpecularResult *= smShadow;
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
		vec4 gTexNorm = texelFetch(g_normal, globalIdx.xy, 0);

		vec3 vNorm = ExtractNormal(gTexNorm);
		float fDepthBufferDepth = ExtractDepth(globalIdx.xy);

		vec4 vWorldSpacePosition = g_mInvProjViewInvViewport * vec4(float(globalIdx.x) + 0.5f, float(globalIdx.y) + 0.5f, fDepthBufferDepth, 1.0f);
		vec3 vPositionWS = vWorldSpacePosition.xyz * (1.0f / vWorldSpacePosition.w);
		vec3 vViewDir = normalize(g_vCameraPos - vPositionWS);

		vec4 AccumDiffuse = vec4(0.0f);
		vec4 AccumSpecular = vec4(0.0f);

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

				DoPointLighting(int(nLightIndex), vNorm, vPositionWS, vViewDir, gTexNorm.a, LightColorDiffuseResult, LightColorSpecularResult);

				AccumDiffuse += LightColorDiffuseResult;
				AccumSpecular += LightColorSpecularResult;
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

				DoSpotLighting(int(nLightIndex), vNorm, vPositionWS, vViewDir, gTexNorm.a, LightColorDiffuseResult, LightColorSpecularResult);

				AccumDiffuse += LightColorDiffuseResult;
				AccumSpecular += LightColorSpecularResult;
			}
		}


		/*// pump up the lights
    AccumDiffuse *= 2;
    AccumSpecular *= 8;
	*/

		vec4 g_AmbientColorUp = vec4(0.013f, 0.015f, 0.050f, 1.0f);
		vec4 g_AmbientColorDown = vec4(0.0013f, 0.0015f, 0.0050f, 1.0f);
		float fAmbientBlend = 0.5f * vNorm.y + 0.5;
		vec4 Ambient = g_AmbientColorUp * fAmbientBlend + g_AmbientColorDown * (1 - fAmbientBlend);
		vec4 DiffuseAndAmbient = AccumDiffuse + Ambient;

		vec4 DiffuseTex = texelFetch(g_diffuse, globalIdx.xy, 0);
		vec4 SpecularTex = texelFetch(g_specular, globalIdx.xy, 0);
		float fSpecMask = SpecularTex.a;//reflective intensity
		
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		compute_paralel_light(light_data[0], vNorm, vPositionWS, gTexNorm.a, DiffuseTex, SpecularTex, ambient, diffuse, specular);
		DiffuseAndAmbient += diffuse;
		AccumSpecular += specular;

		//vec4 Result = DiffuseTex * (DiffuseAndAmbient + AccumSpecular * fSpecMask * SpecularTex);
		vec4 ResultDiffuseAndAmbient = DiffuseTex * DiffuseAndAmbient;
		vec4 ResultSpecular = DiffuseTex * AccumSpecular * fSpecMask * SpecularTex;

		//ResultDiffuseAndAmbient = tmp + ResultDiffuseAndAmbient * 0.1f;
		// vec4(pow(texelFetch(gSpotLightShadowMap, ivec3(globalIdx.xy, 5), 0).r
		imageStore(g_spec_albedro, globalIdx.xy, ResultSpecular);//ResultDiffuseAndAmbient + ResultSpecular
		imageStore(g_albedro, globalIdx.xy, ResultDiffuseAndAmbient + ResultSpecular);//DiffuseTex);// gTexNorm*vec4(0.5f) + vec4(0.5f));//ResultDiffuseAndAmbient);//
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
//WARNING DEPTH [0;1]!!!!   ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
}vGS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//layout ( location = 0 ) uniform mat4 mProjView;
//////////////////////////////////////////////////////////////////////
void main()
{
	//gl_Position = mProjView * vec4(vWorldPos, 1.0f);
	vGS_IN.vWorldPos = v_position;
	vGS_IN.vTex = v_tex;
}
//////////////////////////////////////////////////////////////////////
   S  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
layout(early_fragment_tests) in;
//////////////////////////////////////////////////////////////////////
//define 
//#define USE_EARLY_FRAGMENT_TEST
//////////////////////////////////////////////////////////////////////
//in variable from geometry shader
in struct PS_IN_BLOCK
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//////////////////////////////////////////////////////////////////////
//out to framebuffer texture
layout(location = 0) out vec4 gShadowColor;
//////////////////////////////////////////////////////////////////////
void main()
{
	gShadowColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
//////////////////////////////////////////////////////////////////////

   ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
//////////////////////////////////////////////////////////////////////
//define 
//#define USE_EARLY_FRAGMENT_TEST
//////////////////////////////////////////////////////////////////////
//in variable from geometry shader
in struct PS_IN_BLOCK
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout(location = 17, binding = 0) uniform sampler2D gDiffTexture;
//////////////////////////////////////////////////////////////////////
//out to framebuffer texture
layout(location = 0) out vec4 gShadowColor;
//////////////////////////////////////////////////////////////////////
void main()
{
	float fAlphaTransparenty = texture(gDiffTexture, vPS_IN.vTex).a;
	if (fAlphaTransparenty < 0.6f) discard;

	gShadowColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
//////////////////////////////////////////////////////////////////////
  Х  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
#define SHADOW_MAP_MAX_COUNT 16
//////////////////////////////////////////////////////////////////////
layout(triangles) in;
layout(triangle_strip, max_vertices = SHADOW_MAP_MAX_COUNT * 3) out;
//////////////////////////////////////////////////////////////////////
in struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
}vGS_IN[];
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct PS_IN_BLOCK 
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout ( location = 0 ) uniform mat4 mProjView[SHADOW_MAP_MAX_COUNT];
layout ( location = 16 ) uniform uint uShadowMapCount;
//////////////////////////////////////////////////////////////////////
void main()
{
	for (int i = 0; i < uShadowMapCount; i++)
	{
		for (int j = 0; j < gl_in.length(); j++)
		{
			gl_Layer = i;
			gl_Position = mProjView[i] * vec4(vGS_IN[j].vWorldPos, 1.0f);
			vPS_IN.vTex = vGS_IN[j].vTex;
			EmitVertex();
		}
		EndPrimitive();
	}
}
//////////////////////////////////////////////////////////////////////
   Х  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
#define SHADOW_MAP_MAX_COUNT 16
//////////////////////////////////////////////////////////////////////
layout(triangles) in;
layout(triangle_strip, max_vertices = SHADOW_MAP_MAX_COUNT * 3) out;
//////////////////////////////////////////////////////////////////////
in struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
}vGS_IN[];
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct PS_IN_BLOCK 
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout ( location = 0 ) uniform mat4 mProjView[SHADOW_MAP_MAX_COUNT];
layout ( location = 16 ) uniform uint uShadowMapCount;
//////////////////////////////////////////////////////////////////////
void main()
{
	for (int i = 0; i < uShadowMapCount; i++)
	{
		for (int j = 0; j < gl_in.length(); j++)
		{
			gl_Layer = i;
			gl_Position = mProjView[i] * vec4(vGS_IN[j].vWorldPos, 1.0f);
			vPS_IN.vTex = vGS_IN[j].vTex;
			EmitVertex();
		}
		EndPrimitive();
	}
}
//////////////////////////////////////////////////////////////////////
   S  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
layout(early_fragment_tests) in;
//////////////////////////////////////////////////////////////////////
//define 
//#define USE_EARLY_FRAGMENT_TEST
//////////////////////////////////////////////////////////////////////
//in variable from geometry shader
in struct PS_IN_BLOCK
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//////////////////////////////////////////////////////////////////////
//out to framebuffer texture
layout(location = 0) out vec4 gShadowColor;
//////////////////////////////////////////////////////////////////////
void main()
{
	gShadowColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
//////////////////////////////////////////////////////////////////////

   ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
//////////////////////////////////////////////////////////////////////
//define 
//#define USE_EARLY_FRAGMENT_TEST
//////////////////////////////////////////////////////////////////////
//in variable from geometry shader
in struct PS_IN_BLOCK
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout(location = 17, binding = 0) uniform sampler2D gDiffTexture;
//////////////////////////////////////////////////////////////////////
//out to framebuffer texture
layout(location = 0) out vec4 gShadowColor;
//////////////////////////////////////////////////////////////////////
void main()
{
	float fAlphaTransparenty = texture(gDiffTexture, vPS_IN.vTex).a;
	if (fAlphaTransparenty < 0.6f) discard;

	gShadowColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
//////////////////////////////////////////////////////////////////////
  ?  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec2 v_tex;
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
}vGS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//layout ( location = 0 ) uniform mat4 mProjView;
//////////////////////////////////////////////////////////////////////
void main()
{
	//gl_Position = mProjView * vec4(vWorldPos, 1.0f);
	vGS_IN.vWorldPos = v_position;
	vGS_IN.vTex = v_tex;
}
//////////////////////////////////////////////////////////////////////
   ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
#define SHADOW_MAP_MAX_COUNT 16
#define CUBE_MAP_FACE_COUNT 6
#define MATRIX_MAX_COUNT SHADOW_MAP_MAX_COUNT * CUBE_MAP_FACE_COUNT
//////////////////////////////////////////////////////////////////////
layout(triangles, invocations = CUBE_MAP_FACE_COUNT) in;
layout(triangle_strip, max_vertices = CUBE_MAP_FACE_COUNT * 3) out;
//////////////////////////////////////////////////////////////////////
in struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
	int mInstanceID;
}vGS_IN[];
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct PS_IN_BLOCK 
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout ( location = 1 ) uniform mat4 mProjView[MATRIX_MAX_COUNT];
//layout ( location = MATRIX_MAX_COUNT) uniform uint uShadowMapCount;
//////////////////////////////////////////////////////////////////////
void main()
{
	//gl_InstanceID;
	//gl_InvocationID;
	//gl_Layer;
	//mInstanceID;
	//int mId = gl_InstanceID * CUBE_MAP_FACE_COUNT + gl_InvocationID;

	for (int j = 0; j < gl_in.length(); j++)
	{
		int mId = vGS_IN[j].mInstanceID * CUBE_MAP_FACE_COUNT + gl_InvocationID;

		gl_Layer = mId;
		gl_Position = mProjView[mId] * vec4(vGS_IN[j].vWorldPos, 1.0f);
		vPS_IN.vTex = vGS_IN[j].vTex;
		EmitVertex();
	}
	EndPrimitive();
}
//////////////////////////////////////////////////////////////////////
   S  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
layout(early_fragment_tests) in;
//////////////////////////////////////////////////////////////////////
//define 
//#define USE_EARLY_FRAGMENT_TEST
//////////////////////////////////////////////////////////////////////
//in variable from geometry shader
in struct PS_IN_BLOCK
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//////////////////////////////////////////////////////////////////////
//out to framebuffer texture
layout(location = 0) out vec4 gShadowColor;
//////////////////////////////////////////////////////////////////////
void main()
{
	gShadowColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
//////////////////////////////////////////////////////////////////////

   ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
//layout(early_fragment_tests) in;
//////////////////////////////////////////////////////////////////////
//define 
//#define USE_EARLY_FRAGMENT_TEST
//////////////////////////////////////////////////////////////////////
//in variable from geometry shader
in struct PS_IN_BLOCK
{
	vec2 vTex;
}vPS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
layout(location = 0, binding = 0) uniform sampler2D gDiffTexture;
//////////////////////////////////////////////////////////////////////
//out to framebuffer texture
layout(location = 0) out vec4 gShadowColor;
//////////////////////////////////////////////////////////////////////
void main()
{
	float fAlphaTransparenty = texture(gDiffTexture, vPS_IN.vTex).a;
	if (fAlphaTransparenty < 0.6f) discard;

	gShadowColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
//////////////////////////////////////////////////////////////////////
   R  ,   S H A D E R   џџ       0         #version 440 core
//////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;
//////////////////////////////////////////////////////////////////////
//out to pixel variable
out struct GS_IN_BLOCK
{
	vec3 vWorldPos;
	vec2 vTex;
	int mInstanceID;
}vGS_IN;
//////////////////////////////////////////////////////////////////////
//uniform variable
//layout ( location = 0 ) uniform mat4 mProjView;
//////////////////////////////////////////////////////////////////////
void main()
{
	//gl_Position = mProjView * vec4(vWorldPos, 1.0f);
	vGS_IN.vWorldPos = v_position;
	vGS_IN.vTex = v_tex;
	vGS_IN.mInstanceID = gl_InstanceID;
}
//////////////////////////////////////////////////////////////////////
  О  ,   S H A D E R   џџ       0         #version 440 core
////////////////////////////////////////////////////
//layout(early_fragment_tests) in; 
////////////////////////////////////////////////////
#define SSR_ITTERATION 32
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
	float fDepth = texelFetch(gDepth, iPosition, 0).r;
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
	float fDepth = texelFetch(gSourceDepth, iPosition, 0).r;
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
	float mLDel = 0.004f;
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
	float mFresnel = max(dot(mWorldNorm, mEyeDir), 0.0f);//clamp(0.4f + 2.8f * pow(1.0f + dot(mWorldNorm, mEyeDir), 2.0f), 0.0f, 1.0f);
	mError = 1.0f - clamp(L * mLDel, 0.0f, 1.0f);
	float mCorrect = mFresnel * mError;
	//////////////////////////////////////////////////////////////////////
	return texture(gSourceImage, mUVZ.xy, 0.4f + 3.0f * (1.0f - mCorrect)) * mCorrect + texture(gSourceImage, mUVZ.xy + mReflectDir.xz * 0.4f, 5.0f * (1.0f - mCorrect)) *(1.0f - mCorrect);
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
	gResColor = mDiffuse + mSpecular;// mReflection * 4.0f;
}

  v  ,   S H A D E R   џџ       0         #version 440 core
////////////////////////////////////////////////////
layout (location = 0) in vec2 v_tex;
////////////////////////////////////////////////////
out vec2 gTex;
////////////////////////////////////////////////////
void main()
{
	gTex = v_tex;
	vec3 vWPosition = vec3(v_tex * vec2(2.0f) + vec2(-1.0f), 0.0f);
	gl_Position = vec4(vWPosition, 1.0f);
}   
  ,   S H A D E R   џџ       0         яЛП#version 440 core
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


