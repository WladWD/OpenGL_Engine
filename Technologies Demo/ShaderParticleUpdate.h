#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderParticleUpdate : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		GLint g_dt_s[mprog_count];
		GLint g_TotalVel[mprog_count];
		GLint g_EyePos[mprog_count];
		GLint g_HeightRange[mprog_count];
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderParticleUpdate(void);
		~ShaderParticleUpdate();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetDtSecond(float dt);
		////////////////////////////////////////////////////
		void SetTotalVel(glm::vec3 vector);
		void SetEyePos(glm::vec3 vector);
		void SetHeightRage(float val);
	};
}
/*layout (location = 0) in vec3	v_pos;
layout (location = 1) in vec3	v_seed;
layout (location = 2) in vec3	v_speed;
layout (location = 3) in float	v_random;
layout (location = 4) in uint	v_Type;
////////////////////////////////////////////////////
layout (location = 0)uniform float g_dt_s;
layout (location = 1)uniform vec3 g_TotalVel;
layout (location = 2)uniform vec3 g_EyePos;
layout (location = 3)uniform float g_HeightRange;*/