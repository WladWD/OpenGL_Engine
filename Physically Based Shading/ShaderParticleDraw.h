#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderParticleDraw : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		GLint g_SpriteSize[mprog_count];
		GLint g_dt_s[mprog_count];
		GLint g_TotalVel[mprog_count];
		GLint g_eyePos[mprog_count];
		GLint g_mProjViewWorld[mprog_count];
		GLint g_SunLight[mprog_count];
		GLint g_MoonLight[mprog_count];
		GLint g_drop_array[mprog_count];
		GLuint g_drop_tex_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderParticleDraw(void);
		~ShaderParticleDraw();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		void BindTextureToUnit(void);
		////////////////////////////////////////////////////
		void SetProjViewWorld(glm::mat4 matrix);
		////////////////////////////////////////////////////
		void SetTotalVel(glm::vec3 vector);
		void SetEyePos(glm::vec3 vector);
		void SetSunLight(glm::vec4 color);
		void SetMoonLight(glm::vec4 color);
		void SetDropTextures(void);
		void SetDtSecond(float val);
		void SetSpriteSize(float val);
	};
}
/*layout (location = 0)uniform float g_SpriteSize; //0.8;
layout (location = 1)uniform float g_dt_s;
layout (location = 2)uniform vec3 g_TotalVel;
layout (location = 3)uniform vec3 g_eyePos;
layout (location = 4)uniform mat4 g_mProjViewWorld;
layout (location = 5)uniform vec4 g_SunLight;
layout (location = 6)uniform vec4 g_MoonLight;*/