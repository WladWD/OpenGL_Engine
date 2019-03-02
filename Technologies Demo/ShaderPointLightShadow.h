#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderPointLightShadow : public ShaderBase
	{
#define mprog_count 2
		////////////////////////////////////////////////////
		//static const int mprog_count = 2;//���������� �������� ��� �������� �������
		////////////////////////////////////////////////////
		GLint mProjView[mprog_count];
		//GLint uShadowMapCount[mprog_count];
		////////////////////////////////////////////////////
		GLint gDiffTexture[mprog_count];
		////////////////////////////////////////////////////
		GLint Diffuse_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderPointLightShadow(void);
		~ShaderPointLightShadow();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		GLuint GetProgramClip(void);
		////////////////////////////////////////////////////
		void SetProjViewMatrix(const glm::mat4 matrix[], uint32_t count);
		//void SetShadowCount(uint32_t count);
		////////////////////////////////////////////////////
		void SetDiffuseTex(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
		////////////////////////////////////////////////////
	};
}

//http://steps3d.narod.ru/tutorials/texture-swizzle-tutorial.html
