#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderGrass : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 2;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint gWorld[mprog_count];
		GLint gProjView[mprog_count];
		GLint g_StarIntensity[mprog_count];
		GLint g_cam_pos[mprog_count];
		////////////////////////////////////////////////////
		GLint m_diffuse[mprog_count];
		GLint m_specular[mprog_count];
		GLint m_emissive[mprog_count];
		////////////////////////////////////////////////////
		GLint g_diffuse[mprog_count];
		GLint g_normal[mprog_count];
		////////////////////////////////////////////////////
		GLint diff_slot, norm_slot;
		////////////////////////////////////////////////////
		GLint g_lightDir[mprog_count];//mast be normalize
		GLint g_lightDiffuse[mprog_count];
		GLint g_lightSpecular[mprog_count];
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderGrass(void);
		~ShaderGrass();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
		////////////////////////////////////////////////////
		void SetProjView(glm::mat4 matrix);
		void SetWorld(glm::mat4 matrix);
		void SetCamPos(glm::vec3 vector);
		void SetStarIntensity(float star_intensity);
		////////////////////////////////////////////////////
		void SetMaterialDiffuse(glm::vec4 color);
		void SetMaterialSpecular(glm::vec4 color);
		void SetMaterialEmissive(glm::vec4 color);
		////////////////////////////////////////////////////
		void SetTextureDiffuse(void);
		void SetTextureNormal(void);
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
layout (location = 9, binding = 1)uniform sampler2D g_normal;*/
