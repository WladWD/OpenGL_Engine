#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class Shader_VNTT : public ShaderBase
	{
#define mprog_count 2
		////////////////////////////////////////////////////
		//static const int mprog_count = 2;//количество программ для текущего шейдера
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
		GLint diff_slot, norm_slot, spec_slot, reflect_slot;
		////////////////////////////////////////////////////
		GLint g_lightDir[mprog_count];//mast be normalize
		GLint g_lightDiffuse[mprog_count];
		GLint g_lightSpecular[mprog_count];
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		Shader_VNTT(void);
		~Shader_VNTT();
		////////////////////////////////////////////////////
		GLuint GetProgramDiff(void);
		GLuint GetProgramDiffReflect(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
		////////////////////////////////////////////////////
		void SetProjView(glm::mat4 matrix);
		void SetCamPos(glm::vec3 vector);
		////////////////////////////////////////////////////
		void SetMaterialDiffuse(glm::vec4 color);
		void SetMaterialSpecular(glm::vec4 color);
		void SetMaterialEmissive(glm::vec4 color);
		////////////////////////////////////////////////////
		void SetTextureDiffuse(void);
		void SetTextureNormal(void);
		void SetTextureSpecular(void);
		void SetTextureReflaction(void);
		////////////////////////////////////////////////////
		void SetLightDir(glm::vec3 vector);
		void SetLightDiffuse(glm::vec4 color);
		void SetLightSpecular(glm::vec4 color);
	};
}
/*layout (location = 0)uniform mat4 g_proj_view;
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
layout (location = 11, binding = 3)uniform samplerCube g_reflaction;*/