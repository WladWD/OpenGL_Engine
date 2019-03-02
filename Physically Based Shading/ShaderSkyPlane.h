#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderSkyPlane : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint g_proj_view[mprog_count];
		GLint g_offset_vec[mprog_count];
		GLint g_cloud_color[mprog_count];
		GLint CloudCover[mprog_count];
		GLint CloudSharpness[mprog_count];
		////////////////////////////////////////////////////
		GLint CloudTexture[mprog_count];
		////////////////////////////////////////////////////
		GLint CloudSlot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderSkyPlane(void);
		~ShaderSkyPlane();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetProjView(glm::mat4 matrix);
		////////////////////////////////////////////////////
		void SetOffsetTex(glm::vec3 vector);
		void SetCloudColor(glm::vec3 vector);
		////////////////////////////////////////////////////
		void SetCloudCover(float val);
		void SetCloudSharpness(float val);
		////////////////////////////////////////////////////
		void SetCloudTexture(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}
/*layout (location = 0) uniform mat4 g_proj_view;
layout (location = 1) uniform vec3 g_offset_vec;
layout (location = 2) uniform vec3 g_cloud_color;
layout (location = 3) uniform float CloudCover;//= -0.1;
layout (location = 4) uniform float CloudSharpness;//= 0.25;
////////////////////////////////////////////////////
layout (location = 5, binding = 0) uniform sampler2D CloudTexture;*/
