#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderDecale : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint gDepth[mprog_count];
		GLint gDecaleTextures[mprog_count];
		GLint gDecaleMatrix[mprog_count];
		GLint gDecaleTextureID[mprog_count];
		GLint mRealDecaleCount[mprog_count];
		GLint gProjViewInv[mprog_count];
		GLint gDiffuse[mprog_count];
		////////////////////////////////////////////////////
		GLint gDepth_slot, gDiffuse_slot, gDecaleTexture_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderDecale(void);
		~ShaderDecale();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetDecaleMatrixes(const glm::mat4 *mMatrix, uint32_t mDecaleCount);
		void SetTextureID(const int32_t *mTextureIds, uint32_t mDecaleCount);
		////////////////////////////////////////////////////
		void SetDepthTexture(void);
		void SetDecaleTextures(void);
		////////////////////////////////////////////////////
		void SetRealDecaleCount(int32_t mDecaleCount);
		void SetProjViewInv(glm::mat4 mProjViewInv);
		////////////////////////////////////////////////////
		GLuint GetDiffuseSlot(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}

