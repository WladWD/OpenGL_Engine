#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderSkyDome : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//���������� �������� ��� �������� �������
		////////////////////////////////////////////////////
		GLint g_proj_view_world[mprog_count];
		GLint InvOpticalDepthNLessOne[mprog_count];
		GLint InvOpticalDepthN[mprog_count];
		GLint InnerRadius[mprog_count];
		GLint OuterRadius[mprog_count];
		GLint mPI[mprog_count];
		GLint NumSamples[mprog_count];
		GLint fScale[mprog_count];
		GLint v2dRayleighMieScaleHeight[mprog_count];
		////////////////////////////////////////////////////
		GLint InvRayleighMieNLessOne[mprog_count];
		GLint v3SunDir[mprog_count];//not init
		GLint KrESun[mprog_count];
		GLint KmESun[mprog_count];
		GLint Kr4PI[mprog_count];
		GLint Km4PI[mprog_count];
		////////////////////////////////////////////////////
		GLint g[mprog_count];
		GLint g2[mprog_count];
		GLint fExposure[mprog_count];
		////////////////////////////////////////////////////
		GLint InvWavelength[mprog_count];//not init
		GLint WavelengthMie[mprog_count];//not init
		////////////////////////////////////////////////////
		GLint starIntensity[mprog_count];
		////////////////////////////////////////////////////
		GLint RayLeighTexture[mprog_count];
		GLint MieTexture[mprog_count];
		GLint StarTexture[mprog_count];
		////////////////////////////////////////////////////
		GLint RayLeigh_slot, Mie_slot, Star_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderSkyDome(void);
		~ShaderSkyDome();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetProjViewWorld(glm::mat4 matrix);
		////////////////////////////////////////////////////
		void SetInvWavelength(glm::vec3 vector);
		void SetWavelengthMie(glm::vec3 vector);
		void Setv3SunDir(glm::vec3 vector);//normalize
		////////////////////////////////////////////////////
		void SetDefValue(void);
		////////////////////////////////////////////////////
		void SetRayLeighTexture(void);
		void SetMieTexture(void);
		void SetStarTexture(void);
		////////////////////////////////////////////////////
		void SetNumSamples(float num_samples_);
		void SetExposure(float val);
		void SetStarIntensity(float val);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}
/*layout (location = 0)uniform float InvOpticalDepthNLessOne = 1.0f/255.0f;
layout (location = 1)uniform float InvOpticalDepthN = 1.0f/256.0f;
layout (location = 2)uniform float InnerRadius = 6356.7523142;
layout (location = 3)uniform float OuterRadius = 6356.7523142 * 1.0157313;
layout (location = 4)uniform float PI = 3.1415159;
layout (location = 5)uniform float NumSamples = 20;
layout (location = 6)uniform float fScale = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);
layout (location = 7)uniform vec2 v2dRayleighMieScaleHeight = vec2(0.25, 0.1);
////////////////////////////////////////////////////
layout (location = 8)uniform vec2 InvRayleighMieNLessOne = vec2(1.0f/255.0f, 1.0f/127.0f);
layout (location = 9)uniform vec3 v3SunDir = vec3( 0, 1, 0 );
//float ESun = 20.0f;
//float Kr = 0.0025f;
//float Km = 0.0010f;
layout (location = 10)uniform float KrESun = 0.0025f * 20.0f;
layout (location = 11)uniform float KmESun = 0.0010f * 20.0f;
layout (location = 12)uniform float Kr4PI = 0.0025f * 4.0f * 3.1415159;
layout (location = 13)uniform float Km4PI = 0.0010f * 4.0f * 3.1415159;
////////////////////////////////////////////////////
layout (location = 14)uniform float g = -0.990;
layout (location = 15)uniform float g2 = (-0.990) * (-0.990);
layout (location = 16)uniform float fExposure = -2.0;
////////////////////////////////////////////////////
layout (location = 17)uniform vec3 InvWavelength;
layout (location = 18)uniform vec3 WavelengthMie;
////////////////////////////////////////////////////
layout (location = 19)uniform float starIntensity = 0.5f;*/