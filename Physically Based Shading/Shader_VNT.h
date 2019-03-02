#include "Shader_Resource_Compiler.h"

#pragma once
namespace Shader
{
//#define to_shader_name(s) #s
	class Shader_VNT
	{
		GLuint shaders[4];
		GLuint program[3];
		////////////////////////////////////////////////////
		int32_t programm_id;
		////////////////////////////////////////////////////
		GLint g_proj_view;
		GLint g_cam_pos;
		////////////////////////////////////////////////////
		GLint m_diffuse;
		GLint m_specular;
		GLint m_emissive;
		////////////////////////////////////////////////////
		GLint g_diffuse, diff_slot;
		GLint g_specular, spec_slot;
		GLint g_reflaction, reflect_slot;
		////////////////////////////////////////////////////
		GLint g_lightDir;//mast be normalize
		GLint g_lightDiffuse;
		GLint g_lightSpecular;
		////////////////////////////////////////////////////
		void InitProgramm(GLuint vs, GLuint ps, GLuint programm);
		void InitVariable(GLuint programm);
	public:
		Shader_VNT(void);
		~Shader_VNT();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
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
layout (location = 9, binding = 1)uniform sampler2D g_specular;//only red chanel
layout (location = 10, binding = 2)uniform samplerCube g_reflaction;*/