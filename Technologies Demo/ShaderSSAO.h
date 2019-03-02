#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"
#include <vector>

#pragma once
namespace Shader {

class ShaderSSAO : public ShaderBase {
#define mprog_count 1

		GLint mProj[mprog_count];
		GLint mInvProj[mprog_count];

		GLint mView[mprog_count];
		GLint mInvView[mprog_count];

		GLint mProjView[mprog_count];
		GLint mInvProjView[mprog_count];
		
		GLint mWorld[mprog_count];
		GLint mInvWorld[mprog_count];

		GLint mProjViewWorld[mprog_count];
		GLint mInvProjViewWorld[mprog_count];

		GLint noiseScale[mprog_count];
		GLint randomVector[mprog_count];

		GLint gDepth[mprog_count];
		GLint gNormal[mprog_count];
		GLint gDiffuse[mprog_count];
		GLint gSpecular[mprog_count];
		GLint gNoise[mprog_count];
				
		GLint gDepth_slot, gNormal_slot, gDiffuse_slot, gSpecular_slot, gNoise_slot;
		
#undef mprog_count
		
		void InitVariable(GLuint program);
		void bindTextureToSlot();
	public:
		ShaderSSAO();
		~ShaderSSAO();
		
		GLuint getProgram();
		
		void setProj(const glm::mat4 &mProj);
		void setInvProj(const glm::mat4 &mInvProj);

		void setView(const glm::mat4 &mView);
		void setInvView(const glm::mat4 &mInvView);

		void setProjView(const glm::mat4 &mProjView);
		void setInvProjView(const glm::mat4 &mInvProjView);

		void setWorld(const glm::mat4 &mWorld);
		void setInvWorld(const glm::mat4 &mInvWorld);

		void setProjViewWorld(const glm::mat4 &mProjViewWorld);
		void setInvProjViewWorld(const glm::mat4 &mInvProjViewWorld);

		void setNoiseScale(const glm::vec2 &texDim);
		void setRandomVectors(const std::vector<glm::vec4>& rVectors);

		void bindDepthTexture(GLuint texture);
		void bindNormalTexture(GLuint texture);
		void bindDiffuseTexture(GLuint texture);
		void bindSpecularTexture(GLuint texture);
		void bindNoiseTexture(GLuint texture);

		static constexpr int kernelSize = 64;
	};
}

