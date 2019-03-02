#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderSkyPlaneUpdate : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		//g_proj_view
		GLint g_proj_view[mprog_count];
		GLint m_offset[mprog_count];
		GLint CloudCover[mprog_count];
		GLint CloudSharpness[mprog_count];
		////////////////////////////////////////////////////
		GLint NoiseTexture[mprog_count];
		////////////////////////////////////////////////////
		GLint Noise_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderSkyPlaneUpdate(void);
		~ShaderSkyPlaneUpdate();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetProjView(glm::mat4 m_mat);
		////////////////////////////////////////////////////
		void SetOffset(glm::vec3 offset);
		void SetCloudCover(float val);
		void SetCloudSharpness(float val);
		////////////////////////////////////////////////////
		void SetNoiseTexture(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}
/*
layout (location = 1) uniform float CloudCover;
layout (location = 2) uniform float CloudSharpness;
layout (location = 3) uniform vec3 m_offset;
////////////////////////////////////////////////////
layout (location = 4, binding = 0) uniform sampler3D NoiseTexture;*/
