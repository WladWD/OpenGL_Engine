#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderSSR : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint gCamPos[mprog_count];
		GLint gCamDir[mprog_count];
		////////////////////////////////////////////////////
		GLint gSourceDepth[mprog_count];
		GLint gSourceImage[mprog_count];
		////////////////////////////////////////////////////
		GLint gProjView_Source[mprog_count];
		GLint gProjViewInv_Source[mprog_count];
		////////////////////////////////////////////////////
		GLint gDepth[mprog_count];
		GLint gNormal[mprog_count];
		GLint gDiffuse[mprog_count];
		GLint gSpecular[mprog_count];
		////////////////////////////////////////////////////
		GLint gProjView[mprog_count];
		GLint gProjViewInv[mprog_count];
		////////////////////////////////////////////////////
		GLint gSourceDepth_slot, gSourceImage_slot;
		GLint gDepth_slot, gNormal_slot, gDiffuse_slot, gSpecular_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderSSR(void);
		~ShaderSSR();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetCamPos(glm::vec3 mCamPos);
		void SetCamDir(glm::vec3 mCamDir);
		////////////////////////////////////////////////////
		void SetSourceDepth(void);
		void SetSourceImage(void);
		////////////////////////////////////////////////////
		void SetProjViewSource(glm::mat4 mProjView);
		void SetProjViewInvSource(glm::mat4 mProjViewInv);
		////////////////////////////////////////////////////
		void SetDepthLayer(void);
		void SetNormalLayer(void);
		void SetDiffuseLayer(void);
		void SetSpecularLayer(void);
		////////////////////////////////////////////////////
		void SetProjView(glm::mat4 mProjView);
		void SetProjViewInv(glm::mat4 mProjViewInv);
		////////////////////////////////////////////////////
		GLuint GetTexCoordAttribLocation(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}

