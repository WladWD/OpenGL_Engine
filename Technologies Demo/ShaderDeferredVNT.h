#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderDeferredVNT : public ShaderBase
	{
#define mprog_count 7
		////////////////////////////////////////////////////
		//static const int mprog_count = 7;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint g_proj_view[mprog_count];
		GLint g_cam_pos[mprog_count];
		////////////////////////////////////////////////////
		GLint m_diffuse[mprog_count];
		GLint m_specular[mprog_count];
		GLint m_emissive[mprog_count];
		////////////////////////////////////////////////////
		GLint g_diffuse[mprog_count];
		GLint g_specular[mprog_count];
		GLint g_reflaction[mprog_count];
		////////////////////////////////////////////////////
		GLint Diffuse_slot, Specular_slot, Reflaction_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderDeferredVNT(void);
		~ShaderDeferredVNT();
		////////////////////////////////////////////////////
		GLuint GetProgramNoTexture(void);
		GLuint GetProgramDiffuse(void);
		GLuint GetProgramDiffuseClip(void);
		GLuint GetProgramDiffSpec(void);
		GLuint GetProgramDiffSpecClip(void);
		GLuint GetProgramDiffSpecRefl(void);
		GLuint GetProgramDiffSpecReflClip(void);
		////////////////////////////////////////////////////
		void SetProjViewWorld(glm::mat4 matrix);
		void SetCamPos(glm::vec3 pos);
		////////////////////////////////////////////////////
		void SetDiffuseColor(glm::vec4 color);
		void SetSpecularColor(glm::vec4 color);
		void SetEmissiveColor(glm::vec4 color);
		////////////////////////////////////////////////////
		void SetDiffuseTex(void);
		void SetSpecularTex(void);
		void SetReflactionTex(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
	/*layout (location = 0)uniform mat4 g_proj_view;
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
layout (location = 0) out vec2 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;*/
}

