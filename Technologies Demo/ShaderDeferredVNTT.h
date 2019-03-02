#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderDeferredVNTT : public ShaderBase
	{
#define mprog_count 6
		////////////////////////////////////////////////////
		//static const int mprog_count = 6;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint g_proj_view[mprog_count];
		GLint g_cam_pos[mprog_count];
		////////////////////////////////////////////////////
		GLint m_diffuse[mprog_count];
		GLint m_specular[mprog_count];
		GLint m_emissive[mprog_count];
		////////////////////////////////////////////////////
		GLint g_diffuse[mprog_count];
		GLint g_normal[mprog_count];
		GLint g_specular[mprog_count];
		GLint g_reflaction[mprog_count];
		////////////////////////////////////////////////////
		GLint Diffuse_slot, Normal_slot, Specular_slot, Reflaction_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderDeferredVNTT(void);
		~ShaderDeferredVNTT();
		////////////////////////////////////////////////////
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
		void SetNormalTex(void);
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
layout (location = 6, binding = 1)uniform sampler2D g_normal;
layout (location = 7, binding = 2)uniform sampler2D g_specular;//only red chanel
layout (location = 8, binding = 3)uniform samplerCube g_reflaction;*/
}

